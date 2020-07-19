file = open("B31_UNI.txt", "r")
oldfile_lines = file.readlines()
tempfile_lines = []
#flag = 1
for key_line in oldfile_lines:
    for i in range(len(oldfile_lines)):
        if key_line.split()[0] == oldfile_lines[i].split()[0]:
            if key_line.split()[2] <= oldfile_lines[i].split()[2]:  
                 key_line = oldfile_lines[i] 
    #check tempfile if the key_line already exist or not
    if key_line in tempfile_lines:
        pass
    else:
        tempfile_lines.append(key_line)
        
with open('B31_Unique.txt', 'w') as f: #It clears the existing file and opens a new clean/empty file
    for item in tempfile_lines:
        f.write("%s" % item)    


f.close()
file.close()
    
