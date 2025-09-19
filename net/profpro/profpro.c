#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/profpro.h>
#include "profpro.h"

static struct sk_buff *build_profpro_packet(struct net_device *dev, struct profpro_addr src, struct profpro_addr dst, void *payload, size_t payload_len){

	struct sk_buff *skb;
	struct profpro_hdr *hdr;

	skb = alloc_skb(sizeof(struct profpro_hdr) + payload_len + LL_RESERVED_SPACE(dev), GFP_ATOMIC);

	if(!skb){

		pr_err("Failed to init socket buffer!\n");
		return NULL;

	}

	skb_reserve(skb, LL_RESERVED_SPACE(dev));

	hdr = (struct profpro_hdr *) skb_put(skb, sizeof(struct profpro_hdr));
	hdr -> version = 1;
	hdr -> ttl = 64;
	hdr -> length = htons(sizeof(struct profpro_hdr) + payload_len);
	hdr -> src.network = htons(src.network);
	hdr -> dst.network = htons(dst.network);
	hdr -> dst.host = htons(dst.host);
	hdr -> checksum  = 0;

	memcpy(skb_put(skb, payload_len), payload, payload_len);

	skb -> protocol = htons(ETH_P_PROFPRO);
	skb -> dev = dev;
	skb_set_network_header(skb,0);

	return skb;
}

static void profpro_test_send(void){

	struct net_device *dev;
	struct profpro_addr src = { .network = 0x0808, .host = 0x0001};
	struct profpro_addr dst = { .network = 0x0808, .host = 0x0002};
	struct sk_buff *skb;

	dev = dev_get_by_name(&init_net, "ens192");

	if(!dev){
		pr_err("ProfPro: Device not found!\n");
		return;
	}

	skb = build_profpro_packet(dev, src, dst, "Hello World", 11);

	if(!skb){
		pr_err("ProfPro: Failed to build packet!\n");
		dev_put(dev);
		return;
	}

	dev_queue_xmit(skb);
	dev_put(dev);
	pr_info("ProfPro: Test packet was sent!\n");

}

static int __init profpro_init(void){

	pr_info("ProfPro: Module was loaded!\n");
	profpro_test_send();
	return 0;

}

static void __exit profpro_exit(void){

	pr_info("ProfPro: Module was unloaded!\n");

}

module_init(profpro_init);
module_exit(profpro_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xavier Wall");
MODULE_DESCRIPTION("Supporting Kernel Module For ProfPro Networking Stack");
