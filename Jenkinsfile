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
                
                // Debug: List files and count
                sh '''
                    echo "Files in src directory:"
                    ls -1 ${WORKSPACE}/src
                    echo "Total files:"
                    ls ${WORKSPACE}/src | wc -l
                '''
            }
        }
        
        stage('Norminette Check') {
            steps {
                script {
                    // Extremely verbose Norminette check with explicit error handling
                    def checkResult = sh(
                        script: """
                            docker run --rm \
                            --network ${NORMINETTE_NETWORK} \
                            -v ${WORKSPACE}/src:${SRC_FOLDER} \
                            ${NORMINETTE_IMAGE} \
                            bash -c "
                                set -x;
                                which norminette;
                                norminette --version;
                                echo 'Scanning files:';
                                find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) -print0 | 
                                    xargs -0 -I {} bash -c 'echo \"Checking file: {}\"; norminette \"{}\" || echo \"Error checking {}\"'
                            "
                        """,
                        returnStatus: true
                    )
                    
                    // Detailed report generation
                    sh """
                        docker run --rm \
                        --network ${NORMINETTE_NETWORK} \
                        -v ${WORKSPACE}/src:${SRC_FOLDER} \
                        ${NORMINETTE_IMAGE} \
                        bash -c "
                            echo 'Norminette Code Quality Report' > /norminette_report.txt;
                            echo '==========================' >> /norminette_report.txt;
                            find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) -print0 | 
                                xargs -0 norminette >> /norminette_report.txt 2>&1;
                            echo '\n\nDetailed Statistics:' >> /norminette_report.txt;
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
