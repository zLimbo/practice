package com.zlimbo.study.blockchain.zlimbochain;

public class Block {

    public static class Header {
        private String version;
        private String parentHash;
    }

    private long size;
    private  Header blockHeader;


}
