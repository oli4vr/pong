# pong
<p>
pong.c
<br />
by Olivier Van Rompuy
</p>
<p>Simple TCP port scan tool.</p>
<p>Created as a simple tcp connection test tool. (Which is sometimes a better test than an icmp "ping")</p>
<p>You can provide a single port, a comma seperated list of ports, a port range or a combination of both</p>
<p>
<pre>
pong
	Simple port scan tool for network connection testing
	by Olivier Van Rompuy
Syntax     :
	pong [-r NRseconds] HOSTNAME|IP [PORT1,PORT2,PORT3,PORTX-PORTY,...] [TIMEOUT]
		-r Repeat every nr of seconds
Examples   :
	pong 127.0.0.1
	pong 127.0.0.1 22
	pong 127.0.0.1 22,80,443
	pong 127.0.0.1 22,6000-6010,443
CSV Output :
	HOST;PORT;ERRNO;ERRNO_DESCRIPTION

</pre>
</p>
# Build
<pre># apt install gcc make
(or yum/zypper/... install)
# make
# make install
-> installs to ~/bin
</pre>