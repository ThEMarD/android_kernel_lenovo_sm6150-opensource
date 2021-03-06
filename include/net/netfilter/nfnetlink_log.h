/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KER_NFNETLINK_LOG_H
#define _KER_NFNETLINK_LOG_H

void
nfulnl_log_packet(struct net *net,
		  u_int8_t pf,
		  unsigned int hooknum,
		  const struct sk_buff *skb,
		  const struct net_device *in,
		  const struct net_device *out,
		  const struct nf_loginfo *li_user,
		  const char *prefix);
void
__nfulnl_log_packet(struct net *net,
		  u_int8_t pf,
		  unsigned int hooknum,
		  const struct sk_buff *skb,
		  const struct net_device *in,
		  const struct net_device *out,
		  const struct nf_loginfo *li_user,
		  const char *prefix,
		  unsigned int layer);

#define NFULNL_COPY_DISABLED    0xff

#endif /* _KER_NFNETLINK_LOG_H */

