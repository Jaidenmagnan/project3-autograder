#!/bin/bash

MOSS_SCRIPT="../moss.sh"
LANGUAGE="cc" # Replace with the language of your choice
BASEFILES="../src/*.cpp" # Adjust path to the base files

SUBMISSIONS_DIR="extracted_submissions"

# Construct the command
CMD="$MOSS_SCRIPT -l $LANGUAGE -d"

# Add base files
for basefile in $BASEFILES; do
    if [ -f "$basefile" ]; then
        CMD+=" -b \"$basefile\""
    fi
done

# Add student directories
for dir in $SUBMISSIONS_DIR/*; do
    if [ -d "$dir" ]; then
        # Add each .cpp file in the src directory
        for file in "$dir"/src/*.cpp; do
            if [ -f "$file" ]; then
                CMD+=" \"$file\""
            fi
        done
    fi
done

# Execute the command
echo "Executing command: $CMD"
eval $CMD

