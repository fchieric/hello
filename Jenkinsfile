pipeline {
    agent any
    
    environment {
        NORMINETTE_EXIT_CODE = 0
        TOTAL_FILES = 0
        FAILED_FILES = 0
        PASSED_FILES = 0
    }
    
    stages {
        stage('Debug Environment') {
            steps {
                script {
                    sh '''
                        echo "Current directory: $PWD"
                        echo "Directory contents:"
                        ls -la
                        echo "src directory contents:"
                        ls -la src/
                        echo "Find test:"
                        find "$(pwd)/src" -type f -name "*.c"
                    '''
                }
            }
        }
        
        stage('Prepare Environment') {
            steps {
                script {
                    env.TOTAL_FILES = sh(
                        script: 'ls src/*.c | wc -l',
                        returnStdout: true
                    ).trim()
                    echo "Found ${env.TOTAL_FILES} C files to check"
                }
            }
        }
        
        stage('Run Norminette') {
            steps {
                script {
                    try {
                        def normOutput = sh(
                            script: '''
                                docker run --rm \\
                                    -v "$(pwd)":/code:ro \\
                                    -w /code \\
                                    ghcr.io/fchieric/norminette-checker:latest \\
                                    sh -c "pwd && ls -la && norminette src/*.c"
                            ''',
                            returnStdout: true
                        )
                        
                        echo "Norminette output: ${normOutput}"
                        
                        env.FAILED_FILES = sh(
                            script: 'echo "${normOutput}" | grep -c "Error!" || echo "0"',
                            returnStdout: true
                        ).trim()
                        
                        env.PASSED_FILES = "${Integer.parseInt(env.TOTAL_FILES) - Integer.parseInt(env.FAILED_FILES)}"
                        
                        if (env.FAILED_FILES.toInteger() > 0) {
                            error "Norminette ha trovato errori in ${env.FAILED_FILES} file(i)"
                        }
                    } catch (Exception e) {
                        env.NORMINETTE_EXIT_CODE = 1
                        echo "Errore durante l'esecuzione di norminette: ${e.message}"
                        throw e
                    }
                }
            }
        }
    }
    
    post {
        success {
            echo """
            ✅ Norminette Check completato con successo!
            📊 Report:
            - File totali analizzati: ${env.TOTAL_FILES}
            - File che hanno passato: ${env.PASSED_FILES}
            - File con errori: ${env.FAILED_FILES}
            """
        }
        failure {
            echo """
            ❌ Norminette Check fallito!
            📊 Report:
            - File totali analizzati: ${env.TOTAL_FILES}
            - File che hanno passato: ${env.PASSED_FILES}
            - File con errori: ${env.FAILED_FILES}
            """
        }
    }
}
