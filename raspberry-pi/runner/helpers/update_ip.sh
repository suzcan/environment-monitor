#!/bin/bash

# new IP address to update
#NIP="http://$(hostname -I)"
NIP="http://enviro-monitor.local"
filename='old_ip.txt'

# retrieve old IP to replace
while read line; do
	NOP="http://$line"
done < $filename

# update IP address in wordpress database
mysql -u root -psensor -D sense_data -e "UPDATE sd_options SET option_value = replace(option_value, '$NOP', '$NIP') WHERE option_name = 'home' OR option_name = 'siteurl'; UPDATE sd_posts SET guid = replace(guid, '$NOP', '$NIP'); UPDATE sd_posts SET post_content = replace(post_content, 'NOP', '$NIP');"

# update ip being used by wordpress database
./store_new_ip
