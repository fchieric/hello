pipeline {
    agent any
    
    environment {
        NORMINETTE_EXIT_CODE = 0
        TOTAL_FILES = 0
        FAILED_FILES = 0
        PASSED_FILES = 0
    }
    
    stages {
        stage('Prepare Environment') {
            steps {
                script {
                    // Conta il numero totale di file C
                    env.TOTAL_FILES = sh(
                        script: 'find src/ -type f -name "*.c" | wc -l',
                        returnStdout: true
                    ).trim()
                }
            }
        }
        
        stage('Run Norminette') {
            agent {
                docker {
                    image 'ghcr.io/fchieric/norminette-checker:latest'
                    args '-v ${WORKSPACE}:/workspace'
                }
            }
            steps {
                script {
                    try {
                        // Esegui norminette e cattura l'output
                        def normOutput = sh(
                            script: 'cd /workspace && norminette src/',
                            returnStdout: true
                        )
                        
                        // Conta i file falliti
                        env.FAILED_FILES = sh(
                            script: 'echo "${normOutput}" | grep -c "Error!"',
                            returnStdout: true
                        ).trim() ?: "0"
                        
                        // Calcola i file che hanno passato il test
                        env.PASSED_FILES = "${Integer.parseInt(env.TOTAL_FILES) - Integer.parseInt(env.FAILED_FILES)}"
                        
                        // Se ci sono file falliti, marca lo stage come fallito
                        if (env.FAILED_FILES.toInteger() > 0) {
                            error "Norminette ha trovato errori in ${env.FAILED_FILES} file(i)"
                        }
                    } catch (Exception e) {
                        env.NORMINETTE_EXIT_CODE = 1
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
