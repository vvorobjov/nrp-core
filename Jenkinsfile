#!groovy
// Load shared library at master branch
// the path to the repo with this library should be specified in Jenkins
// https://tomd.xyz/jenkins-shared-library/
// https://www.jenkins.io/doc/book/pipeline/shared-libraries/
@Library('nrp-shared-libs@master') _

pipeline {
    environment {
        TOPIC_BRANCH = selectTopicBranch(env.BRANCH_NAME, env.CHANGE_BRANCH)
        BITBUCKET_DOC_REPO = "${env.BRANCH_NAME == "master" ? "hbpneurorobotics" : "nrp-core-dev-docs"}"
    }
    agent {
        dockerfile {
            label 'ci_label'
            // --net=host solves the problem with gRPC resolving localhost to IPv6 ("Address family not supported" errors)
            args '-u nrpuser:nrpgroup --privileged --net=host'
        }
    }
    options {
        // Abort a build on timeout
        timeout(time: 3, unit: 'HOURS') 
    }

    stages {
       
        stage('Prepare Build') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'CMake nrp-core')

                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/10-prepare-build.sh'
            }
        }
       
        stage('Build') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Building nrp-core')

                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/20-build.sh'
            }
        }
       
        stage('Build Docs') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Building documentation')

                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/25-build_docs.sh'
                script{
                    if (fileExists('build/pages_with_subsections_and_subpages.txt')) {
                        unstable(message: "${STAGE_NAME} some doc pages are corrupted")
                        archiveArtifacts artifacts: 'build/pages_with_subsections_and_subpages.txt', fingerprint: true
                    }
                }
            }
        }
       
        stage('Unit tests') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Testing nrp-core')

                // run simultaneously just one set of tests
                lock("${NODE_NAME}-nrp-core-unit-tests") {
                    // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                    sh 'bash .ci/30-run-tests.sh'
                    cobertura coberturaReportFile: 'build/gcovr.xml'
                }
            }
        }
       
        stage('Static tests') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Testing nrp-core')

                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/40-run-cppcheck.sh'
            }
        }
       
        stage('Publishing results') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Publishing results for nrp-core')

                    xunit (
                    thresholds: [
                        skipped(failureThreshold: '3'),
                        failed(failureThreshold: '0')
                        ],
                    tools: [CTest(
                        pattern: 'build/Testing/**/*.xml',
                        deleteOutputFiles: true,
                        failIfNotNew: false,
                        skipNoTestFiles: false,
                        stopProcessingIfError: true
                        ),
                        JUnit(
                        pattern: 'build/xml/*.xml',
                        deleteOutputFiles: true,
                        failIfNotNew: false,
                        skipNoTestFiles: false,
                        stopProcessingIfError: true
                        )]
                    )
                publishCppcheck pattern:'build/cppcheck/cppcheck_results.xml'
            }
        }

        stage('Publishing Doxygen docs') {
            when {
                expression { (env.TOPIC_BRANCH == "development") || (env.TOPIC_BRANCH == "master") }
            }
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Updating documentation')

                sshagent(['vorobev_key']) {
                    sh("""
                        #!/usr/bin/env bash
                        set +x

                        git config --global user.name 'nrp-jenkins'
                        git config --global user.email 'neurorobotics@ebrains.eu'

                        export GIT_SSH_COMMAND='ssh -oStrictHostKeyChecking=no'

                        git clone git@bitbucket.org:${BITBUCKET_DOC_REPO}/${BITBUCKET_DOC_REPO}.bitbucket.io.git

                        cp -rf build/doxygen/html/* ${BITBUCKET_DOC_REPO}.bitbucket.io/
                        cd ${BITBUCKET_DOC_REPO}.bitbucket.io
                        if [[ -z \$(git status --porcelain) ]]; then
                            echo 'Nothing to commit!'
                        else
                            git add -A
                            git commit -m '[NRRPLT-0000] Jenkins automatic doc-pages update'
                            git push
                        fi
                    """)
                }
            }
        }

        stage('Publishing Sphinx docs') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Updating documentation')

                // This step requires nexusArtifactUploader plugin
                // The artifact will be uploaded to 
                // <protocol>://nexusUrl/repository/<repository>/<groupId>/<artifactId>/<version>/<artifactId>-<version>-<classifier>.<type>
                nexusArtifactUploader(
                    nexusVersion: 'nexus3',
                    protocol: 'https',
                    nexusUrl: "${env.NEXUS_REGISTRY_IP}",
                    groupId: 'nrp-core',
                    version: "${TOPIC_BRANCH}",
                    repository: 'nrp-storage',
                    credentialsId: 'nexusadmin',
                    artifacts: [
                        [artifactId: "nrp-core-docs",
                        classifier: 'docs',
                        file: 'build/nrp-core-rst.zip',
                        type: 'zip']
                    ]
                )

                // Start Jenkins pipeline to publish docs to neurorobotics.net
                build job: '/Deploy_nrp_core_docs', parameters: [
                    string(name: 'BRANCH_NAME', value:"${env.TOPIC_BRANCH}"), 
                    string(name: 'BASE_BRANCH_NAME', value:"development"), 
                    string(name: 'ADMIN_SCRIPT_BRANCH', value:"nrp-core"), 
                    string(name: 'DOCUMENTATION_REF', value:"nrp-core"), 
                    booleanParam(name: 'DEPLOY', value: true), 
                    booleanParam(name: 'RELEASE', value: false), 
                    booleanParam(name: 'LATEST', value: false)], wait: false
            }
        }
    }

    post {
        always {
            cleanWs()
        }
        aborted {
            bitbucketStatusNotify(buildState: 'FAILED', buildDescription: 'Build aborted!')
        }
        failure {
            bitbucketStatusNotify(buildState: 'FAILED', buildDescription: 'Build failed, see console output!')
        }
        success{
            bitbucketStatusNotify(buildState: 'SUCCESSFUL', buildDescription: 'branch ' + env.BRANCH_NAME)
        } 
    }
}
