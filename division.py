def divAlg(a,b):
    aCopy = a
    bCopy = b
    result = 0
    divHelp = 1
    powOf2 = 1
    
    if(b == 0 or a == 0):
        return 0   
    
    while(aCopy >= bCopy):  
        bCopy *= 2
        divHelp *= 2
        powOf2 += 1
    
    while(powOf2 > 0):
        print("------------------")
        print("powOf2: ", powOf2)
        print("aCopy: ", aCopy)
        print("bCopy: ", bCopy)
        print("aCopy >= bCopy: ", aCopy >= bCopy)
        print("-------------------")
        if(aCopy >= bCopy):
            aCopy -= bCopy
            result += divHelp
        bCopy /= 2
        print("bCopy: ", bCopy)
        divHelp /= 2
        powOf2 -= 1
    
    return result, aCopy

def mulAlg(a,b):
    aCopy = a
    bCopy = b
    res = 0
    
    while(bCopy > 1):
        if(bCopy % 2 == 1):
            res+= aCopy
            bCopy -= 1
    
        aCopy *= 2
        bCopy /= 2
    
    res += aCopy
    return res

if __name__ == "__main__":
    a = 1000
    b = 200
    #get div result and remainder
    result = divAlg(a, b)
    print(result)   
    result = mulAlg(a, b)
    print(result) 