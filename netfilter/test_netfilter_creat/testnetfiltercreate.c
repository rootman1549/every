#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <net/route.h>
#include <net/icmp.h>
#include <linux/netfilter_ipv4.h>

MODULE_LICENSE("GPL");

#define    ETH     "eth0"
#define    SIP     "192.168.238.180"
#define    DIP     "192.168.1.101"
#define    SPORT   39804
#define    DPORT   80

unsigned char   SMAC[ETH_ALEN] = {0x00,0x0C,0x29,0x4F,0xDE,0xAC};
unsigned char   DMAC[ETH_ALEN] = {0x00,0x50,0x56,0xFA,0x70,0x2A};

int cp_dev_xmit_tcp (char * eth, u_char * smac, u_char * dmac,
                u_char * pkt, int pkt_len, 
                u_long sip, u_long dip, 
                u_short sport, u_short dport, u_long seq, u_long ack_seq, u_char psh, u_char fin)
{
        struct sk_buff * skb = NULL;
        struct net_device * dev = NULL;
        struct ethhdr * ethdr = NULL;
        struct iphdr * iph = NULL;
        struct tcphdr * tcph = NULL;
        u_char * pdata = NULL;
        int nret = 1;

        if (NULL == smac || NULL == dmac) goto out;

        //dev = dev_get_by_name(eth);
        dev = dev_get_by_name(&init_net, eth);
        if (NULL == dev) 
                goto out;
    
        printk("dev name: %s\n", dev->name);

        //skb = alloc_skb (ETH_HLEN + pkt_len + sizeof (struct iphdr) + sizeof (struct tcphdr) + LL_RESERVED_SPACE (dev), GFP_ATOMIC);
        skb = alloc_skb (pkt_len + sizeof (struct iphdr) + sizeof (struct tcphdr) + ETH_HLEN, GFP_ATOMIC);

        if (NULL == skb) 
                goto out;

        //skb_reserve (skb, LL_RESERVED_SPACE (dev));
        skb_reserve (skb, 2);

        skb->dev = dev;
        skb->pkt_type = PACKET_OTHERHOST;
        skb->protocol = __constant_htons(ETH_P_IP);
        skb->ip_summed = CHECKSUM_NONE;
        skb->priority = 0;

        //skb->nh.iph = (struct iphdr*)skb_put(skb, sizeof (struct iphdr));
        //skb->h.th = (struct tcphdr*)skb_put(skb, sizeof (struct tcphdr));

        skb_put(skb, sizeof(struct ethhdr));
        skb_reset_mac_header(skb);

        skb_put(skb, sizeof(struct iphdr));
        //skb_reset_network_header(skb);
        skb_set_network_header(skb, sizeof(struct ethhdr));

        skb_put(skb, sizeof(struct tcphdr));
        //skb_reset_transport_header(skb);
        skb_set_transport_header(skb, sizeof(struct iphdr) + sizeof(struct ethhdr));

        pdata = skb_put (skb, pkt_len);

        {
                if (NULL != pkt) 
                        memcpy (pdata, pkt, pkt_len);
        }


        {
                //tcph = (struct tcphdr *) skb->h.th;
                tcph = (struct tcphdr *)skb_transport_header(skb);
                memset (tcph, 0, sizeof (struct tcphdr));
                tcph->source = sport;
                tcph->dest = dport;
                tcph->seq = seq;
                tcph->ack_seq = ack_seq;
                tcph->doff = 5;
                tcph->psh = psh;
                tcph->fin = fin;
                tcph->syn = 1;
                tcph->ack = 0;
                tcph->window = __constant_htons (5840);
                skb->csum = 0;
                tcph->check = 0;
        }

        {
                //iph = (struct iphdr*) skb->nh.iph;
                iph = (struct iphdr*)skb_network_header(skb);
                memset(iph, 0, sizeof(struct iphdr));
                iph->version = 4;
                iph->ihl = sizeof(struct iphdr)>>2;
                iph->frag_off = 0;
                iph->protocol = IPPROTO_TCP;
                iph->tos = 0;
                iph->daddr = dip;
                iph->saddr = sip;
                iph->ttl = 0x40;
                iph->tot_len = __constant_htons(skb->len);
                iph->check = 0;
                iph->check = ip_fast_csum(iph, iph->ihl);
        }

        
        {
                int i = 0;

                printk("len0: %02x\n\n", skb->len);
                
                for (; i < skb->len; i++)
                {
                    if (i != 0 && i % 16 == 0)
                    {
                        printk("\n");
                    }
                    //printk("%02x ", ((unsigned char *)ethdr)[i]);
                    printk("%02x ", skb->data[i]);
                }

                printk("\n");
        }

        //skb->csum = skb_checksum (skb, ETH_HLEN + iph->ihl*4, skb->len - iph->ihl * 4, 0);
        //tcph->check = csum_tcpudp_magic (sip, dip, skb->len - iph->ihl * 4, IPPROTO_TCP, skb->csum);
        skb->csum = skb_checksum (skb, ETH_HLEN + iph->ihl*4, pkt_len + sizeof(struct tcphdr), 0);
        tcph->check = csum_tcpudp_magic (sip, dip, pkt_len + sizeof(struct tcphdr), IPPROTO_TCP, skb->csum);

        {
                int i = 0;

                printk("len1: %02x\n\n", skb->len);
                
                for (; i < skb->len; i++)
                {
                    if (i != 0 && i % 16 == 0)
                    {
                        printk("\n");
                    }
                    //printk("%02x ", ((unsigned char *)ethdr)[i]);
                    printk("%02x ", skb->data[i]);
                }

                printk("\n");
        }

        //skb->mac.raw = skb_push (skb, 14);
        //skb_push(skb, 14);
        //skb_reset_mac_header(skb);
        
        { 
                //ethdr = (struct ethhdr *)skb->mac.raw;
                ethdr = (struct ethhdr *)skb_mac_header(skb);
                memcpy (ethdr->h_dest, dmac, ETH_ALEN);
                memcpy (ethdr->h_source, smac, ETH_ALEN);
                ethdr->h_proto = __constant_htons (ETH_P_IP);
        }


        {
                int i = 0;

                printk("len2: %02x\n\n", skb->len);
                
                for (; i < skb->len; i++)
                {
                    if (i != 0 && i % 16 == 0)
                    {
                        printk("\n");
                    }
                    //printk("%02x ", ((unsigned char *)ethdr)[i]);
                    printk("%02x ", skb->data[i]);
                }
                printk("\n");
        }

        if (0 > dev_queue_xmit(skb)) goto out;
        printk("aaaaaaaaaa1\n");
        nret = 0;
out:
        if (0 != nret && NULL != skb) {dev_put (dev); kfree_skb (skb);}

        printk("aaaaaaaaaaaa2\n");
        return (nret);
}

static int __init init(void)
{
        printk("%s\n","insmod skb_diy module\n");

        cp_dev_xmit_tcp (ETH, SMAC, DMAC,NULL, 0, 
                        in_aton(SIP),in_aton(DIP),
                        htons(SPORT),htons(DPORT),
                        0, 0, 0, 0);
        return 0;
}

static void __exit fini(void)
{
        printk("%s\n","remove skb_diy module.\n");
}

module_init(init);
module_exit(fini);
