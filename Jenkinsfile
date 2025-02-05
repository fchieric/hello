pipeline {
    agent any
    
    environment {
        PATH = "/home/jenkins/.local/bin:$PATH"
        NORMINETTE_EXIT_CODE = 0
        TOTAL_FILES = 0
        FAILED_FILES = 0
        PASSED_FILES = 0
    }
    
    stages {
        stage('Install minikube and kubectl') {
            steps {
                sh '''
                    # Install to user's home directory instead of system
                    mkdir -p $HOME/.local/bin
                    
                    # Download minikube
                    curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64
                    chmod +x minikube-linux-amd64
                    mv minikube-linux-amd64 $HOME/.local/bin/minikube
                    
                    # Download kubectl
                    KUBECTL_VERSION=$(curl -L -s https://dl.k8s.io/release/stable.txt)
                    curl -LO "https://dl.k8s.io/release/${KUBECTL_VERSION}/bin/linux/amd64/kubectl"
                    chmod +x kubectl
                    mv kubectl $HOME/.local/bin/kubectl
                '''
            }
        }
        
        stage('Start minikube') {
            steps {
                sh 'minikube start --driver=docker'
            }
        }
        
        stage('Prepare norminette check') {
            steps {
                script {
                    // conta il numero totale di file C
                    env.TOTAL_FILES = sh(
                        script: 'find src/ -type f -name "*.c" | wc -l',
                        returnStdout: true
                    ).trim()
                    //.trim serve a togliere spazi in caso ci siano
                }
            }
        }
        
        stage('Norminetting') {
            agent {
                kubernetes {
                    yaml '''
                    apiVersion: v1
                    kind: Pod
                    spec:
                      containers:
                      - name: norminette
                        image: ghcr.io/fchieric/norminette-checker:latest
                        command:
                        - cat
                        tty: true
                    '''
                    //cat e tty per evitare che si chiuda il container
                }
            }
            steps {
                // Runna norminette e cattura l' output
                script {
                    try {
                        def normResult = sh(
                            script: 'norminette src/',
                            returnStatus: true
                        )
                        
                        // Lista dei file che falliscono
                        def failedFiles = sh(
                            script: 'norminette src/ | grep -c "KO!"',
                            returnStdout: true
                        ).trim()
                        
                        env.FAILED_FILES = failedFiles
                        env.PASSED_FILES = "${Integer.parseInt(env.TOTAL_FILES) - Integer.parseInt(failedFiles)}"
                        env.NORMINETTE_EXIT_CODE = normResult
                        
                        // Se normResult non è zero, significa che alcuni file hanno fallito
                        if (normResult != 0) {
                            error "Norminette found style errors in some files"
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
            echo "Tutto norminetatto ✅"
            echo "File totali: ${env.TOTAL_FILES}"
            echo "File norminettati: ${env.PASSED_FILES}"
            echo "File non norminettati: ${env.FAILED_FILES}"
        }
        
        failure {
            echo "Norminette non superata ❌"
            echo "File totali: ${env.TOTAL_FILES}"
            echo "File norminettati: ${env.PASSED_FILES}"
            echo "File non norminettati: ${env.FAILED_FILES}"
        }
        
        always {
            sh '''
                minikube stop
                minikube delete
            '''
        }
    }
}
