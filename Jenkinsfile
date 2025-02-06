pipeline {
    agent any
    
    environment {
        NORMINETTE_NETWORK = 'jenkins-norminette-network'
        SRC_FOLDER = '/app/src'
        NORMINETTE_IMAGE = 'ghcr.io/fchieric/norminette-checker:latest'
    }
    
    stages {
        stage('Prepare') {
            steps {
                // Pull the public image
                sh "docker pull ${NORMINETTE_IMAGE}"
                
                // Ensure the network exists
                sh 'docker network inspect ${NORMINETTE_NETWORK} || docker network create ${NORMINETTE_NETWORK}'
                
                // Debug: List files
                sh 'ls -la ${WORKSPACE}/src'
            }
        }
        
        stage('Norminette Check') {
            steps {
                script {
                    // Debug: Verbose file finding and Norminette check
                    def checkResult = sh(
                        script: """
                            docker run --rm \
                            --network ${NORMINETTE_NETWORK} \
                            -v ${WORKSPACE}/src:${SRC_FOLDER} \
                            ${NORMINETTE_IMAGE} \
                            sh -c "
                                echo 'Finding files:';
                                find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\);
                                echo '\n\nRunning Norminette:\n';
                                find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) -print0 | xargs -0 norminette
                            "
                        """,
                        returnStatus: true
                    )
                    
                    // Generate detailed report
                    sh """
                        docker run --rm \
                        --network ${NORMINETTE_NETWORK} \
                        -v ${WORKSPACE}/src:${SRC_FOLDER} \
                        ${NORMINETTE_IMAGE} \
                        sh -c "
                            echo 'Norminette Code Quality Report' > /norminette_report.txt;
                            echo '==========================' >> /norminette_report.txt;
                            find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) -print0 | xargs -0 norminette | tee -a /norminette_report.txt;
                            echo \"\nDetailed Statistics:\" >> /norminette_report.txt;
                            find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) | wc -l >> /norminette_report.txt
                        "
                    """
                    
                    // Copy report to Jenkins workspace
                    sh 'docker cp $(docker ps -lq):/norminette_report.txt ${WORKSPACE}/norminette_report.txt'
                    
                    // Always show the report
                    sh 'cat ${WORKSPACE}/norminette_report.txt'
                    
                    // Fail the build if Norminette check fails
                    if (checkResult != 0) {
                        error "Norminette check failed"
                    }
                }
            }
        }
    }
    
    post {
        always {
            // Remove network
            sh 'docker network rm ${NORMINETTE_NETWORK} || true'
        }
        
        failure {
            echo "Norminette check failed. Please review the report."
        }
        
        success {
            echo "Norminette check completed successfully!"
        }
    }
}
