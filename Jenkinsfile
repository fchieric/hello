pipeline {
    agent any
    
    stages {
        stage('Setup') {
            steps {
                sh 'apt-get install -y python3-pip python3-venv'
                sh 'python3 -m pip install --break-system-packages norminette'
            }
        }
        
        stage('Run Norminette') {
            steps {
                sh '''
                    total_files=0
                    passed_files=0
                    failed_files=0
                    
                    for file in src/*.c; do
                        if [ -f "$file" ]; then
                            ((total_files++))
                            if norminette "$file" > temp_result.txt 2>&1; then
                                ((passed_files++))
                                echo "[OK] $file"
                            else
                                ((failed_files++))
                                echo "[KO] $file"
                                cat temp_result.txt
                            fi
                        fi
                    done
                    
                    echo "=== Norminette Report ==="
                    echo "Total files: $total_files"
                    echo "Passed: $passed_files"
                    echo "Failed: $failed_files"
                    
                    rm -f temp_result.txt
                    [ $failed_files -eq 0 ]
                '''
            }
        }
    }
}
