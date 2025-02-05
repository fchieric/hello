pipeline {
    agent any
    stages {
        stage("Show file") {
            steps {
               script {
                    def branchName = env.BRANCH_NAME
                    echo "Branch corrente: ${branchName}"
                    sh 'ls -la'
               }   
            }
        }
    }
    post {
        success {
            echo 'Pipeline completata con successo!'
        }
        failure {
            echo 'Pipeline fallita!'
        }
    }
}
