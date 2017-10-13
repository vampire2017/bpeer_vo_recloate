ps -ef | grep create_database | grep -v grep | awk '{print $2}' | xargs kill -9
