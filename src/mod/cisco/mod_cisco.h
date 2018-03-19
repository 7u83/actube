#ifndef __MOD_CISCO_H
#define __MOD_CISCO_H


int cisco_in_wtp_descriptor(struct cw_ElemHandler *eh, struct cw_ElemHandlerParams * params, uint8_t * data,
                         int len);

int cisco_out_wtp_descriptor(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
                        , uint8_t * dst);

#endif
