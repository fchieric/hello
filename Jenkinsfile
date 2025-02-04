pipeline {
    agent any
    stages {
        stage("Show file") {
            steps {
               script {
                    def branchName = env.BRANCH_NAME
                    echo "Branch corrente: ${branchName}"
                    bat """
                        echo Lista dei file nel branch ${branchName}:
                        dir /s /b
                    """
               }   
            }
        }
        success {
            echo 'Pipeline completata con successo!'
        }
        failure {
            echo 'Pipeline fallita!'
        }
    }
}
