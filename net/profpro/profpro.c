#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/profpro.h>
#include <profpro.h>

static struct sk_buff *build_profpro_packet(struct net_device *dev, struct profpro_addr src, struct profpro_addr dst, void *payload, size_t payload_len){

	struct sk_buff *skb;
	struct profpro_hdr *hdr;

	skb = alloc_skb(sizeof(struct profpro_hdr) + payload_len + LL_RESERVED_SPACE(dev), GFP_ATOMIC)

	if(!skb)
		return null;

	skb_reserve(skb, LL_RESERVED_SPACE(dev));

	hdr = (struct profpro_hdr *) skb_put(skb, sizeof(struct profpro_hdr));
	hdr -> version = 1;
	hdr -> ttl = 64;
	hdr -> length = htons(sizeof(struct profpro_hdr) + payload_len);
	hdr -> src.network = htons(src.network);
	hdr -> dst.network = htons(dst.network);
	hdr -> dst.host = htons(dst.host);
	hdr -> checksum  = 0;

	memcpy(skb_put(skb, payload_len), paylaod, payload_len);

	skb -> protocol = htons(ETH_P_PROFPRO);
	skb -> dev = dev;
	skb_set_network_header(skb,0);

	return skb;
}
