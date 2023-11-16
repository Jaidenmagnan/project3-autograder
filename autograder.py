import os
import zipfile
import subprocess
from PIL import Image
import glob
import csv


def unzip_all(directory: str, names: list, late: dict):
    # Iterate through all the files in the specified directory
    for filename in os.listdir(directory):
        # Construct the full file path
        file_path = os.path.join(directory, filename)

        # Check if the file is a ZIP file
        if zipfile.is_zipfile(file_path):
            # Open the ZIP file
                
            student_name = filename.split("_")[0]

            late[student_name] = False
            for n in filename.split("_"):
                if n == "LATE":
                    late[student_name] = True
                    


            target_path = f"extracted_submissions/{student_name}"
            with zipfile.ZipFile(file_path, 'r') as zip_ref:
                # Extract all the contents into the directory
                names.append(student_name)
                zip_ref.extractall(target_path)

def compile_c_code(directory: str ) -> bool:
    # Run the command
    os.chdir(directory)
    result_cmake = subprocess.run(["pwd"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    print(result_cmake.stdout)
    cpp_files = glob.glob("src/*.cpp")
    result_cmake = subprocess.run(["g++", "-o", "image-processor"]+cpp_files, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    #print(result_cmake.stdout)
    #print(result_cmake.stderr)
    os.chdir("../..")

    found = False
    ran_correctly = True
    for _, _, files in os.walk(directory):
        if("image-processor" in files):
                found = True
                os.chdir(directory)
                subprocess.run(["cp", "-r", "../../template_files/input", "."])
                subprocess.run(["cp", "-r", "../../template_files/output", "."])
                subprocess.run(["cp", "-r", "../../template_files/examples", "."])
                result_run= subprocess.run(["./image-processor"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                result_run= subprocess.run(["rm", "image-processor"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                os.chdir("../..")

                if result_run.returncode != 0:
                    ran_correctly = False


    return result_cmake.returncode == 0 and found and ran_correctly


def check_rust_code(directory: str) -> bool:
    for root, dirs, files in os.walk(directory):
        if("Cargo.toml" in files):
            os.chdir(directory)
            copy_run = subprocess.run(["cp", "-r", "../../template_files/input", "."])
            copy_run = subprocess.run(["cp", "-r", "../../template_files/output", "."])
            copy_run = subprocess.run(["cp", "-r", "../../template_files/examples", "."])
            result_run= subprocess.run(["cargo", "run"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            os.chdir("../..")
            return True

    return False


def compare_tga(file1, file2):
    # Open the TGA files
    with Image.open(file1) as img1, Image.open(file2) as img2:
        # Ensure the images are the same size
        if img1.size != img2.size:
            return 0
            
        total_pixels = 0
        bad_pixels = 0

        # Compare each pixel
        width, height = img1.size
        for x in range(width):
            for y in range(height):
                total_pixels+=1
                if img1.getpixel((x, y)) != img2.getpixel((x, y)):
                    bad_pixels+=1


        return (float((float(total_pixels-bad_pixels)) / float(total_pixels)))


def main():
    names = list()
    late = {}
    unzip_all("submissions", names, late)

    compiled = {}
    correct = {}
    grades = {}

    for name in names:
        compiled[name] = compile_c_code(f"extracted_submissions/{name}")
        compiled[name] = compiled[name] or check_rust_code(f"extracted_submissions/{name}")
        correct[name] = 0
        grades[name] = 0


    for name in names:
        if compiled[name]:
            for root, dirs, files in os.walk(f"extracted_submissions/{name}/output"):
                num_files = 0
                score = 0.0
                for file in files:
                    try:
                        out = compare_tga(f"extracted_submissions/{name}/output/{file}", f"extracted_submissions/{name}/examples/EXAMPLE_{file}")
                        score += out 
                        if out == 1.0:
                            correct[name] += 1
                        else:
                            print(out)
                    except:
                        pass
                        

            grades[name] = round((float(score) / 12.0)*100, 2) # a perfect score is a 108
        else:
            grades[name] = "-1"

                

            

    subprocess.run(["rm", "-r", "extracted_submissions"])
    
    # Open a file in write mode
    with open('output.csv', 'w', newline='') as csvfile:
        # Create a CSV writer object
        writer = csv.writer(csvfile)

        # Write the header
        writer.writerow(['name', 'score', 'compiled', 'images-correct', 'late'])

        # Write the data
        for name in sorted(names):
            if(grades[name] == 0):
                grades[name] = -1
                compiled[name] = False
            writer.writerow([name, grades[name], compiled[name], correct[name], late[name]])



main()
