pipeline {
    agent any
    stages {
        stage('Install minikube') {
            steps {
                sh '''
                    curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64
                    chmod +x minikube-linux-amd64
                    mv minikube-linux-amd64 /usr/local/bin/minikube
                    curl -LO "https://dl.k8s.io/release/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl"
                    chmod +x kubectl
                    mv kubectl /usr/local/bin/kubectl
                '''
            }
        }
        stage('Start minikube') {
            steps {
                sh 'minikube start --driver=docker'
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
                            # cat -> sovrascrive il comando predefinito del container per evitare che termini, mantenendo il container in esecuzione
                            # tty: true -> Alloca un terminale per il container: consente a Jenkins di eseguire comandi nel container
                    '''
                }
            }
            steps {
                container('norminette') {
                    sh 'norminette src/'
                }
            }
        }
    }
    post {
        success {
            echo 'Yeeeee pipeline completata!'
        }
        failure {
            echo 'Pipeline fallita :( !'
        }
        always {
            sh 'minikube stop'
        }
    }
}
