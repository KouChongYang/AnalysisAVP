# AnalysisAVP
Analysis of audio and video protocols

## H264（新一代视频压缩编码标准）

### H264码流分层

![H.264码流分层结构](/images/H.264码流分层结构.png)

* NAL层，视频数据网络抽象层（Network Abstraction Layer）
* VCL层，视频数据编码层（Video Coding Layer）

### 码流基本概念

![RBSP与SODB](/images/RBSP与SODB.png)

* SODB，数据位串（String Of Data Bits）。原始数据比特流，长度不一定是8的倍数，故需要补齐。由VCL层产生。

* RBSP，原始字节序列负载（Raw Byte Sequence Playload）。SODB + trailing bits，算法是如果SODB最后一个字节不对齐，则补1和多个0。

![NALU](/images/NALU.png)

![NAL_Header](/images/NAL_Header.png)

* NALU，NAL单元。NAL Header（1B）+ RBSP。
