import os

def do_the_thing() -> list[str]:
    the_answer = []
    
    for thing in os.listdir("./"):
        if os.path.isdir(thing) or thing.lower() == "file_manager.py":
            continue
        the_answer.append(thing)
    return the_answer

def write_to_file(the_data : list[str] = do_the_thing(), file_name : str = f"image_names.txt") -> None:
    
    with open(f"{file_name}", f"w") as open_file:
        for index in range(len(the_data)):
            if ".mp4" in the_data[index].lower():
                continue
            the_string = "\t`..${system_slash}${file_directory}${system_slash}" + f"{the_data[index]}`"
            open_file.write(the_string + (",\n" if index + 1 < len(the_data) else "\n"))
        

write_to_file(do_the_thing())