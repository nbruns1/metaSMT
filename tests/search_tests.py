#!/usr/bin/python

import sys
import re
import os

def getTestSuiteName(str):
    regex = r"BOOST_FIXTURE_TEST_SUITE\([^,]+"
    names =re.search(regex, str)
    return (names.group()[25:])
    
def findTestSuites(str):
    regex = r"BOOST_FIXTURE_TEST_SUITE\([^)]+\)[\s,\S]+BOOST_AUTO_TEST_SUITE_END\(\)"
    matches = re.finditer(regex, str)
    testSuiteList = []
    for matchNum, match in enumerate(matches):
        matchNum = matchNum + 1
        testSuiteList .append([getTestSuiteName(match.group()), match.start() ,  match.end()])
    return testSuiteList

def findTestCases(str):
    regex = r"BOOST_AUTO_TEST_CASE\([^)]+"
    matches = re.finditer(regex, str)
    testCasesList = []
    for matchNum, match in enumerate(matches):
        matchNum = matchNum + 1
        testCasesList .append([match.group()[21:], match.start() ,  match.end()])
    return testCasesList
    
def findIncludedFiles(file):
    folder = os.path.dirname(os.path.realpath(file))
    sourcefile = open(file,"r") 
    includeRegex = r"#include\s+\"[^\"]+\""
    matches = re.finditer(includeRegex, sourcefile.read())
    fileList = []
    for x in matches:
        first = x.group().find("\"");
        second = x.group().find("\"", first+1)
        includeFile = x.group()[first+1:second]
        includeFilePath = folder+"/"+includeFile
        fileList.append(includeFilePath)
    return fileList
if len(sys.argv) != 2:
    print("not the right number of arguments")
    sys.exit();
sourcefileName = sys.argv[1]
fileList = findIncludedFiles(sourcefileName)
for x in fileList:
    fileList += findIncludedFiles(x)
for x in fileList:
    file = open(x, "r")
    fileContent = file.read()
    testSuites = findTestSuites(fileContent)
    testCases = findTestCases(fileContent)
    if(len(testSuites) == 1):
        for testCase in testCases:
            print(testSuites[0][0]+"/"+testCase[0])
    else:
        for testCase in testCases:
            for testSuite in testSuites:
                if(testCase[1]>testSuite[1] & testCase[1]<testSuite[2]):
                    print(testSuite[0]+"/"+testCase[0])
    

