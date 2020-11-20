filein = open("in.cpp", "r")
fileout = open("out.cpp", "w")

# 取函数名并添加前后缀


def fun0():
    for line in filein:
        targetLoc = line.find("Pm5990PortGenericDevice")
        if (targetLoc != -1 and line[0] != " "):
            loc1 = targetLoc + line[targetLoc:].find("::")
            loc2 = line.find("(")
            funcname = line[loc1 + 2: loc2]
    #    	lineout = '''speMenu.RegisterFunction("''' \
    #    				+ funcname \
    #    				+ ''' ", ''' \
    #    				+ "&Pm5990PortGenericDevice::" \
    #    				+ funcname \
    #    				+ ");" \
    #    				+ "\n"
            lineout = funcname + "\n"
            fileout.write(lineout)

# 取函数名并添加前后缀


def fun1():
    for line in filein:
        tmp = line.split(" ")
        # print(tmp)
        if tmp[0] == "virtual":
            funcname = tmp[2]
            classname = "&Pm5990OduGenericDevice::"
        else:
            funcname = tmp[1]
            classname = "&Pm5990PortGenericDevice::"
        loc = funcname.find("(")
        funcname = funcname[:loc]
        print(funcname)
        lineout = '''speMenu.RegisterFunction("''' \
            + funcname \
            + '''", ''' \
            + classname \
            + funcname \
            + ");" \
            + "\n"
        print(lineout)
        fileout.write(lineout)

# 去函数名


def fun01():
    for line in filein:
        loc_end = line.find('(')
        loc_start = line.find(' ')
        lineout = line[loc_start + 1: loc_end]
        print(lineout)
        fileout.write('"' + lineout + '",' + '\n')

# 取特定行（分布间隔相同）的特定字段


def fun2():
    cnt = 0
    suffix = ""
    for line in filein:
        cnt += 1
        lineout = ""
        if (cnt - 4) % 10 == 0:  # 依赖前一次计算的结果
            tmp = line.split("_")
            for keyword in tmp[1:-1]:
                # print(keyword)
                lineout += keyword
                lineout += "_"
            lineout = lineout + suffix + "," + "\n"
            print(lineout)
            fileout.write(lineout)

        suffix = ""
        if (cnt - 3) % 10 == 0:
            tmp = line.split("_")
            tmp = tmp[-1].split(",")
            # print(tmp)
            if tmp[0] == "RX":
                suffix = "RX"
            else:
                suffix = "TX"

# 取注释中的特定字段


def fun3():
    for line in filein:
        lineout = ""
        if line[0] == "/":
            tmp = line.split(" ")
            lineout = tmp[1] + "," + "\n"
            print(lineout)
            fileout.write(lineout)

# 根据enumeration中的项目生成switch case 语句


def fun4():
    for line in filein:
        tmp = line.split("_")
        print(tmp)
        if len(tmp) > 1:
            tmp[0] = tmp[0].strip()
            print(tmp[0])
            enumname = ""
            funcname = "get"
            for ele in tmp:
                loc = ele.find("=")
                if loc == -1:
                    if ele.find("\n") == -1:
                        enumname += (ele + '_')
                        if ele != "RX" and ele != "TX":
                            funcname += ele.title()
                    elif ele.find(',') == -1:
                        enumname += ele[:-1]
                        funcname += (ele[:-1]).title()
                        print(enumname)
                        print(funcname)
                    else:
                        enumname += ele[:-2]
                        funcname += (ele[:-2]).title()
                        print(enumname)
                        print(funcname)
                else:
                    enumname += ele[:loc - 1]
                    funcname += (ele[:loc - 1]).title()
                    print(enumname)
                    print(funcname)
                    break
            funcname += "()"
            lineout = ""
            lineout += "case PortStatsType::" + enumname + ":\n"
            lineout += "counter = myPorts[portId]->" + funcname + ";\n"
            lineout += "break;\n"
            fileout.write(lineout)

# 根据enumeration中的项生成只包含enumeration项的一个语句


def fun5():
    for line in filein:
        tmp = line.split("_")
        print(tmp)
        if len(tmp) > 1:
            tmp[0] = tmp[0].strip()
            enumname = ""
            for ele in tmp:
                loc = ele.find("=")
                if loc == -1:
                    if ele.find("\n") == -1:
                        enumname += (ele + '_')
                    elif ele.find(',') == -1:
                        enumname += ele[:-1]
                        print(enumname)
                    else:
                        enumname += ele[:-2]
                        print(enumname)
                else:
                    enumname += ele[:loc - 1]
                    print(enumname)
                    break
            lineout = ""
##            lineout += "myPortStatsMap.insert(std::pair<int, std::string>(static_cast<int>(PortCouterType::"
#            lineout += enumname
#            lineout += '''), "'''
#            lineout += enumname
#            lineout += '''"));\n'''
#            lineout += "(currentAll.test(PortAlarmType::"
#            lineout += enumname
#            lineout += ''') ? "+" : "-"),\n'''
            lineout += '''speMenu.RegisterEnumValue("'''
            lineout += enumname
            lineout += '''", PmcLogSev::'''
            lineout += enumname
            lineout += ''');\n'''
            fileout.write(lineout)

# 根据enumeration中的项目生成相应的函数


def fun6():
    for line in filein:
        if line.find(',') != -1:
            enumname = line[:-2]
        else:
            enumname = line
        print(enumname)
        lineout = ""
#        lineout += "if(Pm5990GenericDevice::getPortStats(PortStatsType::"
#        lineout += enumname
#        lineout += ", portId, counter))\n{\n    "
        lineout += "Pm5990GenericDevice::getPortStats(PortStatsType::"
        lineout += enumname
        lineout += ", portId, counter);\n"
#        lineout += '''std::cout << setw(42) << ios::left << "'''
#        lineout += enumname
#        lineout += '''"  << ios::dec << setw(12) << ios::right << counter << std::endl;\n'''
        lineout += '''std::cout << "'''
        cnt = 0
        while(cnt < 60):
            if cnt == 0:
                lineout += '+'
            elif cnt == 59:
                lineout += '+'
            else:
                lineout += '-'
            cnt += 1
        lineout += '''" << std::endl;\n'''
        lineout += '''std::cout << "|" << setw(42) << right << "'''
        lineout += enumname
        lineout += '''" << "|" << dec << setw(14) << right << counter
                        << setw(2) << right << "|" << std::endl;\n'''
        fileout.write(lineout)

fun01()
filein.close()
fileout.close()
