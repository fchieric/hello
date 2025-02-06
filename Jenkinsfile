pipeline {
    agent any
    
    stages {
        stage('Setup Python') {
            steps {
                sh '''
                    apt-get update
                    apt-get install -y python3-pip
                '''
            }
        }
        
        stage('Install Norminette') {
            steps {
                sh 'pip3 install --user norminette'
            }
        }
        
        stage('Run Norminette') {
            steps {
                sh '''
                    total_files=0
                    passed_files=0
                    failed_files=0
                    
                    echo "=== Starting Norminette Check ==="
                    
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
                    
                    echo "=== Norminette Report ==="
                    echo "Total files scanned: $total_files"
                    echo "Files passed: $passed_files"
                    echo "Files failed: $failed_files"
                    
                    rm -f temp_result.txt
                    
                    if [ $failed_files -gt 0 ]; then
                        exit 1
                    fi
                '''
            }
        }
    }
}
