/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

/* ports */
#define LWAPP_CONTROL_PORT 12223
#define LWAPP_CONTROL_PORT_STR "12223"

#define LWTH_GET_PREAMBLE(th) (th[0]) 


#define LWMSG_DISCOVERY_REQUEST	1

/*
                  Discovery Response                   2
                  Join Request                         3
                  Join Response                        4
                  Join ACK                             5
                  Join Confirm                         6
                  Unused                             7-9
                  Configure Request                   10
                  Configure Response                  11
                  Configuration Update Request        12
                  Configuration Update Response       13
                  WTP Event Request                   14
                  WTP Event Response                  15
                  Change State Event Request          16
                  Change State Event Response         17
                  Unused                           18-21
                  Echo Request                        22
                  Echo Response                       23
                  Image Data Request                  24
                  Image Data Response                 25
                  Reset Request                       26
                  Reset Response                      27
                  Unused                           28-29
                  Key Update Request                  30
                  Key Update Response                 31
                  Primary Discovery Request           32
*/
