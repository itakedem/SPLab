import collections
from plumbum import local

fileName = "lab10_grades.txt"
cat = local['cat']
sed = local['sed']
awk = local['awk']
wc = local['wc']
sort = local['sort']
uniq = local['uniq']



def getStudentList():
    regex = r's/\([a-Z]*\)[\t].*/\1/g'
    cmd = cat[fileName] | sed[regex]
    return cmd()

def getNumOfStundents():
    regex = r's/\([a-Z]*\)[\t].*/\1/g'
    cmd = cat[fileName] | sed[regex] | wc['-w']
    return cmd()

def getNumOfErrors():
    sed1 = sed[r's/[a-Z]*[\t]\(.*\)/\1/g']
    sed2 = sed[r's/[|]/\n/g']
    sed3 = sed[r's/\([a-Z]*\)[:].*/\1/g']
    cmd = cat[fileName] | sed1 | sed2 | sed3 | awk['NF'] | sort | uniq | wc['-l']
    return cmd()

def getErrorList():
    sed1 = sed[r's/[a-Z]*[\t]\(.*\)/\1/g']
    sed2 = sed[r's/[|]/\n/g']
    sed3 = sed[r's/\([a-Z]*\)[:].*/\1/g']
    cmd = cat[fileName] | sed1 | sed2 | sed3 | awk['NF'] | sort | uniq['-c']
    return cmd()

if __name__ == '__main__':
    print(f"students:\n{getStudentList()}\n\n")
    print(f"NumOfStundents:\n{getNumOfStundents()}\n\n")
    print(f"errors:\n{getErrorList()}\n\n")
    print(f"NumOfErrors:\n{getNumOfErrors()}\n\n")

