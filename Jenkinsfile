pipeline {
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
                    # cat -> sovrascrive il comando predefinito del container per evitare che termini, mantenendo il container in esecuz
                    # tty: true -> Alloca un terminale per il container: consente a Jenkins di eseguire comandi nel container
            '''
        }
    }
    
    stages {
        stage('Norminetting') {
            steps {
                container('norminette') {
                            sh 'norminette src/'
                    }
                }
            }
        }
    }
}
