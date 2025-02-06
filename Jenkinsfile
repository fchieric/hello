pipeline {
    agent any
    
    environment {
        NORMINETTE_NETWORK = 'jenkins-norminette-network'
        SRC_FOLDER = '/app/src'
        NORMINETTE_IMAGE = 'ghcr.io/fchieric/norminette-checker:latest'
    }
    
    stages {
        stage('Norminette Check') {
            steps {
                script {
                    // Verbose Norminette check
                    def checkResult = sh(
                        script: """
                            docker run --rm \
                            --network ${NORMINETTE_NETWORK} \
                            -v ${WORKSPACE}/src:${SRC_FOLDER} \
                            ${NORMINETTE_IMAGE} \
                            bash -c '
                                set -e;
                                echo "Norminette version: $(norminette --version)";
                                echo "Scanning files:";
                                for file in $(find /app/src -type f \\( -name "*.c" -o -name "*.h" \\)); do
                                    echo "Checking file: $file";
                                    norminette "$file" || echo "Error checking $file";
                                done
                            '
                        """,
                        returnStatus: true
                    )
                    
                    // Generate report
                    sh """
                        docker run --rm \
                        --network ${NORMINETTE_NETWORK} \
                        -v ${WORKSPACE}/src:${SRC_FOLDER} \
                        ${NORMINETTE_IMAGE} \
                        bash -c '
                            echo "Norminette Code Quality Report" > /norminette_report.txt;
                            echo "==========================" >> /norminette_report.txt;
                            for file in $(find /app/src -type f \\( -name "*.c" -o -name "*.h" \\)); do
                                echo "Checking $file:" >> /norminette_report.txt;
                                norminette "$file" >> /norminette_report.txt 2>&1;
                            done;
                            echo "\n\nDetailed Statistics:" >> /norminette_report.txt;
                            find /app/src -type f \\( -name "*.c" -o -name "*.h" \\) | wc -l >> /norminette_report.txt
                        '
                    """
                    
                    // Copy and display report
                    sh 'docker cp $(docker ps -lq):/norminette_report.txt ${WORKSPACE}/norminette_report.txt'
                    sh 'cat ${WORKSPACE}/norminette_report.txt'
                    
                    // Fail the build if Norminette check fails
                    if (checkResult != 0) {
                        error "Norminette check failed"
                    }
                }
            }
        }
    }
}
