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
            }
        }
        
        stage('Norminette Check') {
            steps {
                script {
                    // Run Norminette check
                    def checkResult = sh(
                        script: """
                            docker run --rm \
                            --network ${NORMINETTE_NETWORK} \
                            -v ${WORKSPACE}/src:${SRC_FOLDER} \
                            ${NORMINETTE_IMAGE} \
                            sh -c "find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) | xargs norminette"
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
                            find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) | while read file; do
                                if norminette \"\$file\"; then
                                    echo \"✅ PASSED: \$file\" >> /norminette_report.txt;
                                else
                                    echo \"❌ FAILED: \$file\" >> /norminette_report.txt;
                                fi
                            done;
                            echo \"\nDetailed Statistics:\" >> /norminette_report.txt;
                            find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) | wc -l >> /norminette_report.txt
                        "
                    """
                    
                    // Copy report to Jenkins workspace
                    sh 'docker cp $(docker ps -lq):/norminette_report.txt ${WORKSPACE}/norminette_report.txt'
                    
                    // Fail the build if Norminette check fails
                    if (checkResult != 0) {
                        error "Norminette check failed"
                    }
                }
            }
        }
        
        stage('Report') {
            steps {
                // Display report
                sh 'cat norminette_report.txt'
                
                // Archive report
                archiveArtifacts artifacts: 'norminette_report.txt', allowEmptyArchive: true
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
