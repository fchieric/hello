pipeline {
    agent any
    
    stages {
        stage('Install Python and Norminette') {
            steps {
                sh '''
                    curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
                    python3 get-pip.py --user
                    python3 -m pip install --user norminette
                '''
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
