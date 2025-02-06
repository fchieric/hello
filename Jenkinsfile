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
                    // Stampa il contenuto della directory per debug
                    sh '''
                        echo "Current directory: $PWD"
                        echo "Directory contents:"
                        ls -la
                        echo "src directory contents:"
                        ls -la src/
                    '''
                }
            }
        }
        
        stage('Prepare Environment') {
            steps {
                script {
                    // Conta il numero totale di file C (correzione del comando find)
                    env.TOTAL_FILES = sh(
                        script: 'find src/ -type f -name "*.c" | wc -l',
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
                        // Test del mount point
                        sh 'docker run --rm -v "$PWD":/workdir -w /workdir alpine ls -la /workdir/src/'
                        
                        // Esegui norminette con path assoluto
                        def normOutput = sh(
                            script: '''
                                docker run --rm \
                                    -v "$PWD":/workdir \
                                    -w /workdir \
                                    ghcr.io/fchieric/norminette-checker:latest \
                                    sh -c "ls -la && norminette src/"
                            ''',
                            returnStdout: true
                        )
                        
                        echo "Norminette output: ${normOutput}"
                        
                        // Conta i file falliti
                        env.FAILED_FILES = sh(
                            script: 'echo "${normOutput}" | grep -c "Error!" || echo "0"',
                            returnStdout: true
                        ).trim()
                        
                        // Calcola i file che hanno passato
                        env.PASSED_FILES = "${Integer.parseInt(env.TOTAL_FILES) - Integer.parseInt(env.FAILED_FILES)}"
                        
                        // Se ci sono file falliti, marca lo stage come fallito
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
