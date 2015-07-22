import telnetlib
import pdb


def telnet_fun(host, port=23, timeout=10):
    '''telnet_fun - telnet to remote host'''
    try:
        tn = telnetlib.Telnet(host, port, timeout)
    except:
        return False
    tn.close()
    return True

def main():
    #pdb.set_trace()

    srcpath = "a.txt"
    dstPath = "b.txt"

    fsrc = open(srcpath, "r")
    fdst = open(dstPath, "a")

    for line in fsrc:
        line = line.strip('\n')
        list_host_port = line.split(',')
        print list_host_port

        host = list_host_port[0]
        port = list_host_port[1]

        if telnet_fun(host, port):
            print "%s%s" % (host, " True")
            want_write_content = "%s%s\n" % (host, " is alive")
        else:
            print "%s%s" % (host, " Fase")
            want_write_content = "%s%s\n" % (host, " is die")

        fdst.write(want_write_content)
         
    fsrc.close()
    fdst.close()

if __name__ == "__main__":
    main()
   #if  telnet_fun("192.168.0.124") == True:
   #     print "True"
