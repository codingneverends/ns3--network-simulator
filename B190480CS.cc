
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

 
using namespace ns3;

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

//------------------- Main Routers 1 & 2 -----------------//

  NodeContainer mainRouters;
  mainRouters.Create (2);

  PointToPointHelper mR_P2P;
  mR_P2P.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  mR_P2P.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer mR_devices;
  mR_devices = mR_P2P.Install (mainRouters);

  InternetStackHelper stack;
  stack.Install (mainRouters);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer mR_interfaces = address.Assign (mR_devices);

//------------------- Main Router 1 and Server1 -----------------//
  NodeContainer mainRouterandServer1;
  mainRouterandServer1.Add(mainRouters.Get(0));
  mainRouterandServer1.Create (1);

  PointToPointHelper mR_S1_P2P;
  mR_S1_P2P.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  mR_S1_P2P.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer mR_S1_devices;
  mR_S1_devices = mR_S1_P2P.Install (mainRouterandServer1);

  stack.Install (mainRouterandServer1.Get(1));

  address.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer mR_S1_interfaces = address.Assign (mR_S1_devices);

//------------------- Main Router 1 and Server2 -----------------//
  NodeContainer mainRouterandServer2;
  mainRouterandServer2.Add(mainRouters.Get(0));
  mainRouterandServer2.Create (1);

  PointToPointHelper mR_S2_P2P;
  mR_S2_P2P.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  mR_S2_P2P.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer mR_S2_devices;
  mR_S2_devices = mR_S2_P2P.Install (mainRouterandServer2);

  stack.Install (mainRouterandServer2.Get(1));

  address.SetBase ("10.1.3.0", "255.255.255.0");

  Ipv4InterfaceContainer mR_S2_interfaces = address.Assign (mR_S2_devices);

//------------------- Main Router 2 and CSMA Server -----------------//
  NodeContainer mainRouterandCSMAServer;
  mainRouterandCSMAServer.Add(mainRouters.Get(1));
  mainRouterandCSMAServer.Create (1);

  PointToPointHelper mR_CSMA_P2P;
  mR_CSMA_P2P.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  mR_CSMA_P2P.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer mR_CSMA_devices;
  mR_CSMA_devices = mR_CSMA_P2P.Install (mainRouterandCSMAServer);

  //comment for csma
  stack.Install (mainRouterandCSMAServer.Get(1));

  address.SetBase ("10.1.4.0", "255.255.255.0");

  Ipv4InterfaceContainer mR_CSMA_interfaces = address.Assign (mR_CSMA_devices);

//------------------- Main Router 2 and Wifi Server -----------------//
  NodeContainer mainRouterandWifiServer;
  mainRouterandWifiServer.Add(mainRouters.Get(1));
  mainRouterandWifiServer.Create (1);

  PointToPointHelper mR_Wifi_P2P;
  mR_Wifi_P2P.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  mR_Wifi_P2P.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer mR_Wifi_devices;
  mR_Wifi_devices = mR_Wifi_P2P.Install (mainRouterandWifiServer);

  stack.Install (mainRouterandWifiServer.Get(1));

  address.SetBase ("10.1.5.0", "255.255.255.0");

  Ipv4InterfaceContainer mR_Wifi_interfaces = address.Assign (mR_Wifi_devices);


//------------------- CSMA Nodes -----------------//
/*
  NodeContainer csmaNodes;
  csmaNodes.Add (mainRouterandCSMAServer.Get (1));
  csmaNodes.Create (4);
  
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);
  stack.Install (csmaNodes);
  
  address.SetBase ("10.1.4.0", "255.255.255.0");

  Ipv4InterfaceContainer mR_CSMA_interfaces = address.Assign (mR_CSMA_devices);

  address.SetBase ("10.1.6.0", "255.255.255.0");

  Ipv4InterfaceContainer CSMA_interfaces = address.Assign (csmaDevices);
*/

//------------------- Wifi Nodes -----------------//
/*
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (3);
  NodeContainer wifiApNode = mainRouterandWifiServer.Get (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy;
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  mobility.Install (wifiStaNodes);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);

  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  address.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer Wifi_AP_interfaces = address.Assign (apDevices);
  Ipv4InterfaceContainer Wifi_Sta_interfaces = address.Assign (staDevices);
*/

  UdpEchoServerHelper echoServer (89);

  ApplicationContainer serverApps = echoServer.Install (mainRouters.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (mR_interfaces.GetAddress (0), 89);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (mainRouterandWifiServer.Get (1));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));
  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}