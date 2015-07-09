#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/if_ether.h>
#include <linux/netfilter_ipv4.h>

MODULE_LICENSE("GPL");

static struct nf_hook_ops ops;

void * packet_init(struct sk_buff *skb, const struct net_device *out)
{
    struct sk_buff *newskb = NULL;
    struct ethhdr *ethh = NULL;
    struct tcphdr *tcph = NULL;
    struct iphdr *iph = NULL;
    unsigned char *pdata = NULL;
    struct tcphdr *old_tcph = NULL;
    struct iphdr *old_iph = NULL;
    struct ethhdr *old_ethh = NULL;
    struct net_device *dev = NULL;
    unsigned short old_data_len = 0;

    unsigned char dest[6] = {0x08, 0x00, 0x27, 0xc4, 0xe6, 0x3b};
    unsigned char src[6] = {0x52, 0x54, 0x00, 0x12, 0x35, 0x02};


    char pkt302[] = 
            "HTTP/1.1 302 Found\r\n"
            "Location: http://www.126.com/\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n\r\n";
    //char pkt301[] = 
    //"HTTP/1.1 301 Moved Permanently\r\n" 
    //"Location: http://www.jd.com\r\n" 
    //"Content-Type: text/html; charset=iso-8859-1\r\n"
    //"Content-length: 0\r\n"
    //"Cache-control: no-cache\r\n"
    //"\r\n";

    // malloc skb space
    // l4
    // l3
    // l2
    // return newskb

    dev = dev_get_by_name(&init_net, "eth0");
    
    {
        // old skb info
        old_tcph = (struct tcphdr *)skb_transport_header(skb);
        old_iph = (struct iphdr *)skb_network_header(skb);
        old_ethh = (struct ethhdr *)skb_mac_header(skb);
    }

    newskb = alloc_skb(strlen(pkt302) + sizeof(struct tcphdr) + sizeof(struct iphdr) + ETH_HLEN + 2, GFP_ATOMIC);
    if (newskb == NULL)
    {
        return NULL;
    }

    skb_reserve(skb, 2);
    
    // skb padding
    newskb->dev = out;
    //newskb->dev = dev;
    newskb->pkt_type = PACKET_HOST;
    newskb->protocol = __constant_htons(ETH_P_IP);
    newskb->ip_summed = CHECKSUM_NONE;
    newskb->priority = 0;

    skb_put(newskb, sizeof(struct ethhdr)); 
    skb_reset_mac_header(newskb);
    skb_put(newskb, sizeof(struct iphdr));
    skb_set_network_header(newskb, sizeof(struct ethhdr));
    skb_put(newskb, sizeof(struct tcphdr));
    skb_set_transport_header(newskb, sizeof(struct iphdr) + sizeof(struct ethhdr));

    //skb_put(newskb, sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct tcphdr));

    pdata = skb_put(newskb, strlen(pkt302));
    if (pdata != NULL)
    {
        memcpy(pdata, pkt302, strlen(pkt302));
    }

    {
        //fill l4
        tcph = (struct tcphdr *)skb_transport_header(newskb);
        memset(tcph, 0, sizeof(struct tcphdr));
        tcph->source = old_tcph->dest;
        tcph->dest = old_tcph->source;
        //tcph->seq = old_tcph->seq;
        //tcph->ack_seq = old_tcph->ack_seq;
        old_data_len = __constant_ntohs(old_iph->tot_len) - old_iph->ihl * 4 - old_tcph->doff * 4;
        printk("---------old seq : %08x\r\n", old_tcph->seq);
        printk("---------old ack : %08x\r\n", old_tcph->ack_seq);
        printk("---------old data_len : %d\r\n", old_data_len);
        tcph->seq = old_tcph->ack_seq;
        //tcph->ack_seq = __constant_htonl(__constant_ntohl(old_tcph->seq) + strlen(pkt302));
        tcph->ack_seq = __constant_htonl(__constant_ntohl(old_tcph->seq) + old_data_len);
        tcph->doff = 5;
        tcph->psh = 1;
        tcph->ack = 1;
        tcph->window = old_tcph->window;
        newskb->csum = 0;
        tcph->check = 0;
        tcph->urg_ptr = 0;
    }

    {
        //fill l3
        iph = (struct iphdr *)skb_network_header(newskb);
        memset(iph, 0, sizeof(struct iphdr));
        iph->version = 4;
        iph->ihl = sizeof(struct iphdr)>>2;
        iph->frag_off = __constant_htons(0x4000);
        iph->protocol = IPPROTO_TCP;
        iph->tos = 0;
        iph->daddr = old_iph->saddr;
        iph->saddr = old_iph->daddr;
        iph->ttl = 0x40;
        iph->tot_len = __constant_htons(strlen(pkt302) + sizeof(struct tcphdr) + sizeof(struct iphdr));
        iph->check = 0;
        iph->check = ip_fast_csum(iph, iph->ihl);
    }

    newskb->csum = skb_checksum (newskb, ETH_HLEN + iph->ihl*4, strlen(pkt302) + sizeof(struct tcphdr), 0);
    tcph->check = csum_tcpudp_magic (iph->saddr, iph->daddr, strlen(pkt302) + sizeof(struct tcphdr), IPPROTO_TCP, newskb->csum);

    {
        ethh = (struct ethhdr *)skb_mac_header(newskb);
        //fill l2
        if (skb->mac_len > 0)
        {
            memcpy(ethh->h_dest, old_ethh->h_source, ETH_ALEN);
            memcpy(ethh->h_source, old_ethh->h_dest, ETH_ALEN);
        }
        else
        {
            //memcpy(ethh->h_dest, old_ethh->h_source, ETH_ALEN);
            //memcpy(ethh->h_source, old_ethh->h_dest, ETH_ALEN);
            //memset(ethh->h_dest, 0, ETH_ALEN);
            //memset(ethh->h_source, 0, ETH_ALEN);
            memcpy(ethh->h_dest, dest, ETH_ALEN);
            memcpy(ethh->h_source, src, ETH_ALEN);
        }
        ethh->h_proto = __constant_htons (ETH_P_IP);
    }

    //skb_pull(newskb, ETH_HLEN);

    return newskb;
}



int url_redirect(struct sk_buff *skb, const struct net_device *out)
{
    // create a pack of skb
    // send to client that it's pack is be hooked
    struct sk_buff * pskb = NULL;

    //if (skb == NULL || out == NULL)
    if (skb == NULL)
    {
        printk("----------------7-3\r\n");
        return -1;
    }

    if (out == NULL)
    {
        printk("---------------7-2\r\n");
        return -1;
    }

    pskb = packet_init(skb, out);
    if (pskb == NULL)
    {
        printk("----------------7-1\r\n");
        return -1;
    }

    printk("----------------7\r\n");

    {
        int i;

        printk("len1: %02x\n\n", pskb->len);
        
        for (i = 0; i < pskb->len; i++)
        {
            if (i != 0 && i % 16 == 0)
            {
                printk("\n");
            }
            //printk("%02x ", ((unsigned char *)ethdr)[i]);
            printk("%02x ", pskb->data[i]);
        }

        printk("\n");
    }

    if (pskb->dev == NULL)
    {
        printk("pskb->dev is null.\r\n");
    }
    //if (dev_queue_xmit(pskb) < 0)
    //{
    //    dev_put(out);
    //    kfree_skb(pskb);
    //    return -2;
    //}
    skb_pull(pskb, ETH_HLEN);
    netif_receive_skb(pskb);
    
    //kfree_skb(pskb);
    printk ("----------------8\r\n");
    return 0;
}


char * get_host_url(void *http_data)
{
    char *tmp = NULL;
    char *host = NULL;
    char *host_end = NULL;
    char *host_url = NULL;
    
    unsigned int host_len = 0;
 
    // todo
    if (http_data == NULL)
        return NULL;
    
    if(memcmp(http_data, "GET /", strlen("GET /")) != 0)
    {
        return NULL;
    }
    
    printk("----------------2\r\n");
    tmp = (char *)http_data;
    host = strstr(tmp, "Host: ");
    if (!host)
    {
        printk("------------------host is null.\r\n");
        return NULL;
    }
    //printk("----------------host:%s\r\n", host);
    host += strlen("Host: ");
    printk("----------------3\r\n");
    host_end = strstr(host, "\r\n");
    if (!host_end)
    {
        printk("----------------host_end is null.\r\n");
        return NULL;
    }
    
    printk("----------------4\r\n");
    host_len = host_end - host;
    printk("----------------host_len:%d\r\n", host_len);
    host_url = kmalloc(host_len + 1, GFP_ATOMIC);
    memset(host_url, 0, host_len + 1);
    memcpy(host_url, host, host_len);
    
    printk("----------------5\r\n");
    //printk("%s\r\n", host_url);
    return host_url;
}

unsigned int nf_hookfn_url_redirect(const struct nf_hook_ops *ops,
                   struct sk_buff *skb,
                   const struct net_device *in,
                   const struct net_device *out,
                   int (*okfn)(struct sk_buff *))
{
    // todo
    // proto tcp
    // if url == www.baidu.com redirect to www.126.com
    // redirect to www.126.com
    //struct ethhdr * ethh = NULL;
    struct iphdr * iph = NULL;
    struct tcphdr * tcph = NULL;

    //unsigned int http_data_len = 0;
    //unsigned char *http_data = NULL;

    char *http_data = NULL;
    char *url = NULL;

    if(skb_linearize(skb)!= 0)
    {
        return NF_DROP;
    }

    iph = ip_hdr(skb);
    tcph = tcp_hdr(skb);

    if (iph->protocol != IPPROTO_TCP)
    {
        return NF_ACCEPT;
    }

    if (!(ntohs(tcph->dest) == 80 || ntohs(tcph->dest) == 8080))
    {
        return NF_ACCEPT;
    }

    //{
    //   int i;

    //   printk("len skb: %02x\n\n", skb->len);

    //   for (i = 0; i < skb->len; i++)
    //   {
    //       if (i != 0 && i % 16 == 0)
    //       {
    //           printk("\n");
    //       }
    //       //printk("%02x ", ((unsigned char *)ethdr)[i]);
    //       printk("%02x ", skb->data[i]);
    //   }

    //   printk("\n");
    //}

    //return NF_ACCEPT;
    //http_data_len = skb->len - iph->ihl * 4 - tcph->doff * 4;
    //http_data = (unsigned char *)kmalloc(http_data_len, GFP_ATOMIC);
    
    // url parse host
    // if url host == www.baidu.com return 0
    // else let it go
    // if 0 redirect url to 126
    
    http_data = (char *)tcph + tcph->doff * 4;

    printk("----------------1\r\n");
    url = get_host_url(http_data);
    if (url == NULL)
    {
        return NF_ACCEPT;
    }

    printk("---------------6\r\n");
    if (0 != strcmp(url, "www.csdn.net"))
    {
        return NF_ACCEPT;
    }
    
    printk("----------------host:%s\r\n", url);
    kfree(url);
    url_redirect(skb, out);

    printk("----------------9\r\n"); 
    //return NF_ACCEPT;
    return NF_STOLEN;
    //return NF_DROP;
}


static int __init init(void)
{
    // hook netfilter
    ops.hook = nf_hookfn_url_redirect;
    ops.hooknum = NF_INET_LOCAL_OUT;
    //ops.hooknum = NF_INET_FORWARD;
    ops.owner = THIS_MODULE;
    ops.pf = PF_INET;
    ops.priority = NF_IP_PRI_FIRST;

    nf_register_hook(&ops);
    
    printk("url_redirect module insmoded.\n");
    
    return 0;
}

static void __exit exit(void)
{
    // unhook netfilter
    nf_unregister_hook(&ops);

    printk("url_redirect module rmmoded.\n");
}

module_init(init);
module_exit(exit);
