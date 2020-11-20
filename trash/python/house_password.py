def checkio(data):

    #replace this for solution
    a = 0
    b = 0
    c = 0
    for e in data:
        if e.isupper(): a = 1
        if e.islower(): b = 1
        if e.isdigit(): c = 1

    return len(data) >= 10 and a == 1 and b == 1 and c == 1

# import re
# ​
# def checkio(data):
    # return all(re.search(regexp, data) for regexp in ('[a-z]', '[A-Z]', '[0-9]', '.{10,}'))

#Some hints
#Just check all conditions


if __name__ == '__main__':
    #These "asserts" using only for self-checking and not necessary for auto-testing
    assert checkio('A1213pokl') == False, "1st example"
    assert checkio('bAse730onE4') == True, "2nd example"
    assert checkio('asasasasasasasaas') == False, "3rd example"
    assert checkio('QWERTYqwerty') == False, "4th example"
    assert checkio('123456123456') == False, "5th example"
    assert checkio('QwErTy911poqqqq') == True, "6th example"
