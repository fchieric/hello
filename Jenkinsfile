pipeline {
    agent any
    
    environment {
        NORMINETTE_NETWORK = 'jenkins-norminette-network'
        SRC_FOLDER = '/app/src'
    }
    
    stages {
        stage('Prepare') {
            steps {
                // Ensure the network exists
                sh 'docker network inspect ${NORMINETTE_NETWORK} || docker network create ${NORMINETTE_NETWORK}'
            }
        }
        
        stage('Norminette Check') {
            steps {
                script {
                    // Run Norminette check in the Norminette container
                    def checkResult = sh(
                        script: """
                            docker run --rm \
                            --network ${NORMINETTE_NETWORK} \
                            -v ${WORKSPACE}/src:${SRC_FOLDER} \
                            norminette \
                            sh -c "find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) | xargs norminette"
                        """,
                        returnStatus: true
                    )
                    
                    // Generate report
                    sh """
                        docker run --rm \
                        --network ${NORMINETTE_NETWORK} \
                        -v ${WORKSPACE}/src:${SRC_FOLDER} \
                        norminette \
                        sh -c "
                            echo 'Norminette Code Quality Report' > norminette_report.txt;
                            echo '==========================' >> norminette_report.txt;
                            find ${SRC_FOLDER} -type f \\( -name '*.c' -o -name '*.h' \\) | while read file; do
                                if norminette \"\$file\"; then
                                    echo \"✅ PASSED: \$file\" >> norminette_report.txt;
                                else
                                    echo \"❌ FAILED: \$file\" >> norminette_report.txt;
                                fi
                            done
                        "
                    """
                    
                    // Copy report to Jenkins workspace
                    sh 'docker cp norminette:/norminette_report.txt ${WORKSPACE}/norminette_report.txt'
                    
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
            // Cleanup
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
