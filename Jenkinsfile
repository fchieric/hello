pipeline {
    agent any
    
    stages {
        stage('Install Norminette') {
            steps {
                sh 'pip3 install --user norminette'
            }
        }
        
        stage('Run Norminette') {
            steps {
                sh '''
                    # Initialize counters
                    total_files=0
                    passed_files=0
                    failed_files=0
                    
                    echo "=== Starting Norminette Check ==="
                    
                    # Run norminette on each .c file
                    for file in src/*.c; do
                        if [ -f "$file" ]; then
                            ((total_files++))
                            if ~/.local/bin/norminette "$file" > temp_result.txt 2>&1; then
                                ((passed_files++))
                                echo "[OK] $file"
                            else
                                ((failed_files++))
                                echo "[KO] $file"
                                cat temp_result.txt
                            fi
                        fi
                    done
                    
                    # Print report
                    echo "=== Norminette Report ==="
                    echo "Total files scanned: $total_files"
                    echo "Files passed: $passed_files"
                    echo "Files failed: $failed_files"
                    
                    # Cleanup
                    rm -f temp_result.txt
                    
                    # Fail if any errors
                    if [ $failed_files -gt 0 ]; then
                        exit 1
                    fi
                '''
            }
        }
    }
}
