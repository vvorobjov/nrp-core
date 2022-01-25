#!groovy
// Load shared library at master branch
// the path to the repo with this library should be specified in Jenkins
// https://tomd.xyz/jenkins-shared-library/
// https://www.jenkins.io/doc/book/pipeline/shared-libraries/
@Library('nrp-shared-libs@master') _

pipeline {
    environment {
        TOPIC_BRANCH = selectTopicBranch(env.BRANCH_NAME, env.CHANGE_BRANCH)
    }
    agent {
        dockerfile {
            label 'ci_label'
            // --net=host solves the problem with gRPC resolving localhost to IPv6 ("Address family not supported" errors)
            args '-u nrpuser:nrpgroup --privileged --net=host'
        }
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
       
        stage('Unit tests') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Testing nrp-core')

                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/30-run-tests.sh'
                cobertura coberturaReportFile: 'build/gcovr.xml'
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

                junit 'build/xml/**/*.xml'
                publishCppcheck pattern:'build/cppcheck/cppcheck_results.xml'
            }
        }
       
        
        stage('Publishing dev docs') {

            // updates dev documentation
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Updating dev documentation')

                sh 'cd build && make nrp_doxygen'

                sshagent(['vorobev_key']) {
                    sh('''
                        #!/usr/bin/env bash
                        set +x

                        git config --global user.name "nrp-jenkins"
                        git config --global user.email "neurorobotics@ebrains.eu"

                        export GIT_SSH_COMMAND="ssh -oStrictHostKeyChecking=no"

                        git clone git@bitbucket.org:nrp-core-dev-docs/nrp-core-dev-docs.bitbucket.io.git

                        cp -rf build/doxygen/html/* nrp-core-dev-docs.bitbucket.io/
                        cd nrp-core-dev-docs.bitbucket.io
                        if [ -z $(git status --porcelain) ];
                        then
                            echo "Nothing to commit!"
                        else
                            git add -A
                            git commit -m "[NRRPLT-0000] Jenkins automatic doc-pages update"
                            git push
                        fi
                    ''')
                }
            }

        }

        stage('Publishing docs') {
            // when master branch, update main documentation
            when {
                expression { env.BRANCH_NAME == "master" }
            }
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Updating main documentation')

                sh 'cd build && make doxygen'

                sshagent(['vorobev_key']) {
                    sh('''
                        #!/usr/bin/env bash
                        set +x

                        git config --global user.name "nrp-jenkins"
                        git config --global user.email "neurorobotics@ebrains.eu"

                        export GIT_SSH_COMMAND="ssh -oStrictHostKeyChecking=no"

                        git clone git@bitbucket.org:hbpneurorobotics/hbpneurorobotics.bitbucket.io.git

                        cp -rf build/doxygen/html/* hbpneurorobotics.bitbucket.io/
                        cd hbpneurorobotics.bitbucket.io
                        if [ -z $(git status --porcelain) ];
                        then
                            echo "Nothing to commit!"
                        else
                            git add -A
                            git commit -m "[NRRPLT-0000] Jenkins automatic doc-pages update"
                            git push
                        fi
                    ''')
                }
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
