//////////////////////////////////////////////////////////////
use pnet::datalink;
//////////////////////////////////////////////////////////////
pub fn ifaddr() {
    let interfaces = datalink::interfaces();
    for interface in interfaces {
        println!( "{:#?}", interface );
    }
}
//////////////////////////////////////////////////////////////
/*
use pnet::datalink::{self, NetworkInterface};
fn main() {
    let interfaces = datalink::interfaces();
    for interface in interfaces {
        println!("Interface: {}", interface.name);
        for ip_network in &interface.ips {
            println!("  IP Address: {}", ip_network.ip());
        }
    }
*/
//////////////////////////////////////////////////////////////
