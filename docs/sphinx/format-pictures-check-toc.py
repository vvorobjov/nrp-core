#!/usr/bin/python3
import sys
import os
import re
from PIL import Image

directory = str(sys.argv[1])

def getImageWidth(imagename):
    im = Image.open(imagename)
    return im.size[0]

def processRST(filepath, directory):
    contents = []
    with open(filepath, 'r') as fp:
        line = fp.readline()
        while line:
            contents.append(line)
            m = re.search(r'\.\. image:: (.*)', line)
            if m:
                imagename = m.group(1)
                w = getImageWidth(os.path.join(directory, imagename))
                contents.append("\t:width: {}\n".format(w))
                contents.append("\t:align: center\n")

            line = fp.readline()

    with open(filepath, 'w') as fp:
        fp.writelines(contents)

def checkTOC(filename):
    # Saparate contents and toc
    # lines with contents of the page
    contents = []
    # lines with contents of the subsections on the page
    subsections = []
    # lines with the TOC of the page
    toc = []
    tocStarted = False
    tocFinished = False
    subStarted = False
    subFinished = False
    headerPassed = False
    with open(filename, 'r') as f:
        lines = f.readlines()
        for line in lines:
            # Wait for the header
            if not headerPassed:
                m = re.search(r'==+', line)
                if m:
                    headerPassed = True
            else:
                if not subStarted and not subFinished:
                    m = re.search(r'\.\. _', line)
                    if m:
                        subStarted = True
                elif not subFinished:
                    m = re.search(r'\.\. toctree::', line)
                    if m:
                        subFinished = True
                if subStarted and not subFinished:
                    subsections.append(line)
                if not tocStarted and not tocFinished:
                    m = re.search(r'\.\. toctree::', line)
                    if m:
                        tocStarted = True
                elif not tocFinished:
                    m = re.search(r'\.\. rubric::', line)
                    if m:
                        tocFinished = True
                if tocStarted and not tocFinished:
                    toc.append(line)
                # Exit if there is subsection and subpages
                if tocStarted and subStarted:
                    return True
            # If we haven't met TOC or subsections, then form content of the page
            if not tocStarted and not subStarted:
                contents.append(line)
    
    return False
    
badPagesFile = "pages_with_subsections_and_subpages.txt"
if os.path.exists(badPagesFile):
    os.remove(badPagesFile)

for file in os.listdir(directory):
     filename = os.fsdecode(file)
     if filename.endswith(".rst"): 
         processRST(os.path.join(directory, filename), directory)
         if filename.startswith("page_") and checkTOC(os.path.join(directory, filename)):
             with open("pages_with_subsections_and_subpages.txt", 'a') as f:
                f.write(filename + "\n")
         continue
     else:
         continue