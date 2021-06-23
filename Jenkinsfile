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
            args '-u root --privileged'
        }
    }
    options { 
        // Skip code checkout prior running pipeline (only Jenkinsfile is checked out)
        skipDefaultCheckout true
    }

    stages {
        stage('Code checkout') {
            steps {
                // Notify BitBucket on the start of the job
                // The Bitbucket Build Status Notifier is used
                // REF: https://plugins.jenkins.io/bitbucket-build-status-notifier/
                
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Code checkout')

                // Debug information on available environment
                echo sh(script: 'env|sort', returnStdout: true)

                checkout scm
                sh 'chown -R "${USER}" ./'
            }
        }
        
        stage('Build') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Building nrp-core')

                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/build.sh'

                junit 'build/xml/**/*.xml'
                publishCppcheck pattern:'build/cppcheck/cppcheck_results.xml'
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
