pipeline {
    agent any    
    environment {
        TOTAL_FILES = '0'
        KO_FILES = '0'
        OK_FILES = '0'
    }   
    stages {
        stage('Conta i file') {
            steps {
                dir('src') {
                    script {
                        TOTAL_FILES = sh(script: 'ls | grep .c | wc -l', returnStdout: true).trim()
                        echo "Numero di file C trovati: ${TOTAL_FILES}"
                    }
                }
            }
        }
        stage('Norminetta') {
            steps {
                dir('src') {
                    script {
                        sh '(norminette *.c) > norminette_output.txt'
                        KO_FILES = sh(script: 'grep -c "Error!" norminette_output.txt || true', returnStdout: true).trim()
                        OK_FILES = "${TOTAL_FILES.toInteger() - KO_FILES.toInteger()}"
                        
                        echo "=== Norminette Report ==="
                        echo "Total files: ${TOTAL_FILES}"
                        echo "Files OK: ${OK_FILES}"
                        echo "Files KO: ${KO_FILES}"
                        
                        sh 'cat norminette_output.txt'
                        sh 'rm norminette_output.txt'
                    }
                }
            }
        }
    }
}
