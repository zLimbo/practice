package com.zlimbo.study.jxta;


import net.jxta.exception.PeerGroupException;
import net.jxta.peergroup.PeerGroup;
import net.jxta.peergroup.PeerGroupFactory;

public class HelloJXTA {

    public static void main(String[] args) {

        System.out.println("Starting JXTA....");

        HelloJXTA myapp = new HelloJXTA();

        myapp.startJXTA();

        System.exit(0);

    }


    public void startJXTA(){

        PeerGroup pg = null;

        try {

            pg = PeerGroupFactory.newNetPeerGroup();

        }catch (PeerGroupException e) {

            System.out.println("Fatal error:group creation failure");

            e.printStackTrace();

            System.exit(1);

        }

        System.out.println("Hello JXTA!:)");

        System.out.println("Group name = "+pg.getPeerGroupName());

        System.out.println("Group ID = "+pg.getPeerGroupID().toString());

        System.out.println("Peer name = "+pg.getPeerName());

        System.out.println("Peer ID = "+pg.getPeerID().toString());

        System.out.println("Peer BaseClass = "+pg.refTcpProtoSpecID.getBaseClass());

    }

}