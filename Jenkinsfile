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
                            --network ${env.NORMINETTE_NETWORK} \
                            -v "\${WORKSPACE}/src:${env.SRC_FOLDER}" \
                            ${env.NORMINETTE_IMAGE} \
                            bash -c 'set -e; \
                                echo "Norminette version: \$(norminette --version)"; \
                                echo "Scanning files:"; \
                                for file in \$(find /app/src -type f \\( -name "*.c" -o -name "*.h" \\)); do \
                                    echo "Checking file: \$file"; \
                                    norminette "\$file" || echo "Error checking \$file"; \
                                done'
                        """,
                        returnStatus: true
                    )
                    
                    // Generate report
                    sh """
                        docker run --rm \
                        --network ${env.NORMINETTE_NETWORK} \
                        -v "\${WORKSPACE}/src:${env.SRC_FOLDER}" \
                        ${env.NORMINETTE_IMAGE} \
                        bash -c 'echo "Norminette Code Quality Report" > norminette_report.txt; \
                            echo "==========================" >> norminette_report.txt; \
                            total_files=\$(find /app/src -type f \\( -name "*.c" -o -name "*.h" \\) | wc -l); \
                            passed=0; \
                            failed=0; \
                            for file in \$(find /app/src -type f \\( -name "*.c" -o -name "*.h" \\)); do \
                                if norminette "\$file" > /dev/null 2>&1; then \
                                    ((passed++)); \
                                else \
                                    ((failed++)); \
                                    echo "❌ Failed: \$file" >> norminette_report.txt; \
                                fi \
                            done; \
                            echo "\\nSummary:" >> norminette_report.txt; \
                            echo "Total files scanned: \$total_files" >> norminette_report.txt; \
                            echo "Passed: \$passed" >> norminette_report.txt; \
                            echo "Failed: \$failed" >> norminette_report.txt'
                    """
                    
                    // Display report
                    sh 'cat norminette_report.txt'
                    
                    // Fail the build if Norminette check fails
                    if (checkResult != 0) {
                        error "Norminette check failed"
                    }
                }
            }
        }
    }
}
