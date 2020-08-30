# AnalysisAVP
Analysis of audio and video protocols

## 音视频录制原理

![音视频录制原理](/images/音视频录制原理.png)

## 音视频播放原理

![音视频播放原理](/images/音视频播放原理.png)

## YUV格式

- 主要用于视频信号的压缩、传输和存储，和向后相容老式黑白电视。
- 其中“Y”表示明亮度（Luminance或Luma），**也称灰阶值**。
- “U”和“V”表示的则是色度（Chrominance或Chroma）**作用是描述影像色彩及饱和度**，用于指定像素的颜色。
- 对于planar的YUV格式，先连续存储所有像素点的Y，紧接着存储所有像素点的U，随后是所有像素点的V。
- 对于packed的YUV格式，每个像素点的Y,U,V是连续存储的。
- **libyuv**，Google开源的实现各种YUV与RGB间相互转换、旋转、缩放的库。
- YUV 4:4:4采样，每一个Y对应一组UV分量。
- YUV 4:2:2采样，每两个Y共用一组UV分量。 
- YUV 4:2:0采样，每四个Y共用一组UV分量。

## 视频主要概念

- **视频码率：** **kb/s**，是指视频文件在单位时间内使用的数据流量，也叫码流率。码率越大，说明单位时间内取样率越大，数据流精度就越高。
- **视频帧率**：fps，通常说一个视频的25帧，指的就是这个视频帧率，即1秒中会显示25帧。帧率越高，给人的视觉就越流畅。
- **视频分辨率**：分辨率就是我们常说的640x480分辨率、1920x1080分辨率，分辨率影响视频图像的大小。
- **I** **帧（**Intra coded frames**）**：

    - I帧不需要参考其他画面而生成,解码时仅靠自己就重构完整图像;

    - I帧图像采用帧内编码方式;

    - I帧所占数据的信息量比较大;

    - I帧图像是周期性出现在图像序列中的，出现频率可由编码器选择;

    - I帧是P帧和B帧的参考帧(其质量直接影响到同组中以后各帧的质量);

    - I帧是帧组GOP的基础帧(第一帧),在一组中只有一个I帧;

    - I帧不需要考虑运动矢量;
- **P** **帧（**Predicted frames**）**：根据本帧与相邻的前一帧（I帧或P帧）的不同点来压缩本帧数据，同时利用了空间和时间上的相关性。
- P帧属于前向预测的帧间编码。它需要参考前面最靠近它的**I**帧或**P**帧来解码。
- **B** **帧（**Bi-directional predicted frames**）**：B 帧图像采用双向时间预测，可以大大提高压缩倍数。

## 音频主要概念

- **采样频率：** 每秒钟采样的点的个数。常用的采样频率有：

    - 22000（22kHz）： 无线广播。

    - 44100（44.1kHz）： CD音质。

    - 48000（48kHz）： 数字电视，DVD。

    - 96000（96kHz）： 蓝光，高清DVD。

    - 192000(192kHz): 蓝光，高清DVD。

- **采样精度（采样深度）** ：每个“样本点”的大小，常用的大小为8bit， 16bit，24bit。

- **通道数：** 单声道，双声道，四声道，5.1声道。

- **比特率：** 每秒传输的bit数，单位为：bps（Bit Per Second）间接衡量声音质量的一个标准。没有压缩的音频数据的比特率 = 采样频率 * 采样精度 * 通道数。

- **码率：** 压缩后的音频数据的比特率。常见的码率：

    - 96kbps： FM质量

    - 128-160kbps：一般质量音频。

    - 192kbps： CD质量。

    - 256-320Kbps：高质量音频

    - 码率越大，压缩效率越低，音质越好，压缩后数据越大。

    - 码率 = 音频文件大小/时长。

- **帧：** 每次编码的采样单元数，比如MP3通常是1152个采样点作为一个编码单元，AAC通常是1024个采样点作为一个编码单元。

- **帧长** ：可以指每帧播放持续的时间：每帧持续时间(秒) = 每帧采样点数 / 采样频率（HZ）比如：MP3 48k, 1152个采样点,每帧则为 24毫秒,1152/48000= 0.024 秒 = 24毫秒；也可以指压缩后每帧的数据长度。

- **交错模式：** 数字音频信号存储的方式。数据以连续帧的方式存放，即首先记录帧1的左声道样本和右声道样本，再开始帧2的记录...

- **非交错模式：** 首先记录的是一个周期内所有帧的左声道样本，再记录所有右声道样本。

- 数字音频压缩编码在保证信号在听觉方面不产生失真的前提下，对音频数据信号进行尽可能大的压缩，降低数据量。数字音频压缩编码采取去除声音信号中冗余成分的方法来实现。所谓冗余成分指的是音频中不能被人耳感知到的信号，它们对确定声音的音色，音调等信息没有任何的帮助。

## 封装概念

- **封装格式** (也叫容器）就是将已经编码压缩好的视频流、音频流及字幕按照一定的方案放到一个文件中，便于播放软件播放。

- **一般来说，视频文件的后缀名就是它的封装格式。**

- **封装的格式不一样，后缀名也就不一样。**

## 音视频同步

- **DTS**（**Decoding Time Stamp**）：即解码时间戳，这个时间戳的意义在于告诉播放器该在什么时候解码这一帧的数据。

- **PTS**（**Presentation Time Stamp**）：即显示时间戳，这个时间戳用来告诉播放器该在什么时候显示这一帧的数据。

- **音视频同步方式：**

    Audio Master：同步视频到音频

    Video Master：同步音频到视频

    External Clock Master：同步音频和视频到外部时钟

    一般情况下 Audio Master > External Clock Master > Video Master 

## H264（新一代视频压缩编码标准）

- H264采⽤了16*16的分块⼤⼩对，视频帧图像进⾏相似⽐较和压缩编码

- H264使⽤帧内压缩和帧间压缩的⽅式提⾼编码压缩率

- H264采⽤了独特的I帧、P帧和B帧策略来实现，连续帧之间的压缩

![h264层次](/images/h264层次.png)

- H264将视频分为连续的帧进⾏传输，在连续的帧之间使⽤I帧、P帧和B帧。同时对于帧内⽽⾔，将图像分块为⽚、宏块和字块进⾏分⽚传输；通过这个过程实现对视频⽂件的压缩包装。

- ⼀个序列的第⼀个图像叫做 IDR 图像（⽴即刷新图像），IDR 图像都是 I 帧图像。I和IDR帧都使⽤帧内预测。I帧不⽤参考任何帧，但是之后的P帧和B帧是有可能参考这个I帧之前的帧的。IDR就不允许这样。其核⼼作⽤是，是为了解码的重同步，当解码器解码到 IDR 图像时，⽴即将参考帧队列清空，将已解码的数据全部输出或抛弃，重新查找参数集，开始⼀个新的序列。这样，如果前⼀个序列出现重⼤错误，在这⾥可以获得重新同步的机会。IDR图像之后的图像永远不会使⽤IDR之前的图像的数据来解码。

![gop](/images/gop.png)

- SPS：序列参数集，SPS中保存了⼀组编码视频序列(Coded video sequence)的全局参数。
- PPS：图像参数集，对应的是⼀个序列中某⼀幅图像或者某⼏幅图像的参数。
- I帧：帧内编码帧，可独⽴解码⽣成完整的图⽚。
- P帧: 前向预测编码帧，需要参考其前⾯的⼀个I 或者B 来⽣成⼀张完整的图⽚。
- B帧: 双向预测内插编码帧，则要参考其前⼀个I或者P帧及其后⾯的⼀个P帧来⽣成⼀张完整的图⽚。  
- 发I帧之前，⾄少要发⼀次SPS和PPS。 

![H.264码流分层结构](/images/H.264码流分层结构.png)

* NAL层，视频数据网络抽象层（Network Abstraction Layer）
* VCL层，视频数据编码层（Video Coding Layer）

![RBSP与SODB](/images/RBSP与SODB.png)

* SODB，数据位串（String Of Data Bits）。原始数据比特流，长度不一定是8的倍数，故需要补齐。由VCL层产生。

* RBSP，原始字节序列负载（Raw Byte Sequence Playload）。SODB + trailing bits，算法是如果SODB最后一个字节不对齐，则补1和多个0。

![NALU](/images/NALU.png)

![NAL_Header](/images/NAL_Header.png)

* NALU，NAL单元。NAL Header（1B）+ RBSP。
* H.264标准指出，当数据流是储存在介质上时，在每个NALU 前添加起始码：0x000001 或
    0x00000001，⽤来指示⼀个NALU 的起始和终⽌位置 。
* 3字节的0x000001只有⼀种场合下使⽤，就是⼀个完整的帧被编为多个slice（⽚）的时
    候，包含这些slice的NALU 使⽤3字节起始码。其余场合都是4字节0x00000001的。  
* H264有两种封装
    - ⼀种是annexb模式，传统模式，有startcode，SPS和PPS是在ES中。
    - ⼀种是mp4模式，⼀般mp4 mkv都是mp4模式，没有startcode，SPS和PPS以及其它信息被封装在container中，每⼀个frame前⾯4个字节是这个frame的⻓度。很多解码器只⽀持annexb这种模式，因此需要将mp4做转换：在ffmpeg中⽤h264_mp4toannexb_filter可以做转换。
* 需要说明的是，通过提⾼GOP值来提⾼图像质量是有限度的，在遇到场景切换的情况时，
    H.264编码器会⾃动强制插⼊⼀个I帧，此时实际的GOP值被缩短了。另⼀⽅⾯，在⼀个GOP中，P、B帧是由I帧预测得到的，当I帧的图像质量⽐较差时，会影响到⼀个GOP中后续P、B帧的图像质量，直到下⼀个GOP开始才有可能得以恢复，所以GOP值也不宜设置过⼤。  
* 由于P、B帧的复杂度⼤于I帧，所以过多的P、B帧会影响编码效率，使编码效率降低。另外，过⻓的GOP还会影响Seek操作的响应速度，由于P、B帧是由前⾯的I或P帧预测得到的，所以Seek操作需要直接定位，解码某⼀个P或B帧时，需要先解码得到本GOP内的I帧及之前的N个预测帧才可以，GOP值越⻓，需要解码的预测帧就越多，seek响应的时间也越⻓。  
* P帧特点:
    1) P帧是I帧后⾯相隔1~2帧的编码帧;
    2) P帧采⽤运动补偿的⽅法传送它与前⾯的I或P帧的差值及运动⽮量(预测误差);
    3) 解码时必须将I帧中的预测值与预测误差求和后才能重构完整的P帧图像;
    4) P帧属于前向预测的帧间编码。它只参考前⾯最靠近它的I帧或P帧;
    5) P帧可以是其后⾯P帧的参考帧,也可以是其前后的B帧的参考帧;
    6) 由于P帧是参考帧,它可能造成解码错误的扩散;
    7) 由于是差值传送,P帧的压缩⽐较⾼。  
* B帧特点
    1）B帧是由前⾯的I或P帧和后⾯的P帧来进⾏预测的;
    2）B帧传送的是它与前⾯的I或P帧和后⾯的P帧之间的预测误差及运动⽮量;
    3）B帧是双向预测编码帧;
    4）B帧压缩⽐最⾼,因为它只反映两参考帧间运动主体的变化情况,预测⽐较准确;
    5）B帧不是参考帧,不会造成解码错误的扩散。  

## AAC（高级音频编码）

![AAC](/images/AAC.png)

![ADIF](/images/ADIF.png)

* ADIF，音频数据交换格式（Audio Data Interchange Format）。这种格式的特征是可以确定的找到这个音频数据的开始，不需进行在音频数据流中间开始的解码，即它的解码必须在明确定义的开始处进行。故这种格式常用在磁盘文件中。

![ADTS](/images/ADTS.png)

![ADTS的固定头信息](/images/ADTS的固定头信息.png)

![ADTS的可变头信息](/images/ADTS的可变头信息.png)

![audio object type](images/audio object type.png)

- profile的值等于 Audio Object Type的值减1

    - profile = MPEG-4 Audio Object Type - 1  

![Sampling Frequencies  ](/images/Sampling Frequencies.png)

![channel configuration](/images/channel configuration.png)

* ADTS，音频数据传输流（Audio Data Transport Stream）。这种格式的特征是它是一个有同步字的比特流，解码可以在这个流中任何位置开始。它的特征类似于mp3数据流格式。

* 每⼀帧的ADTS的头⽂件都包含了⾳频的采样率，声道，帧⻓度等信息，这样解码器才能解析读取。⼀般情况下ADTS的头信息都是7个字节，分为2部分：

    - adts_fixed_header();

    - adts_variable_header();

    其⼀为固定头信息，紧接着是可变头信息。固定头信息中的数据每⼀帧都相同，⽽可变头信息则在帧与帧之间可变。

* ADTS可以在任意帧解码，也就是说它每⼀帧都有头信息。ADIF只有⼀个统⼀的头，所以必须得到所有的数据后解码。

## FLV

![flv文件结构](/images/flv文件结构.png)

![flv详细文件结构](/images/flv详细文件结构.png)

- FLV(Flash Video)是Adobe公司推出的⼀种流媒体格式，由于其封装后的⾳视频⽂件体积⼩、封装简单等特点，⾮常适合于互联⽹上使⽤。⽬前主流的视频⽹站基本都⽀持FLV。  

- FLV封装格式是由⼀个⽂件头(file header)和 ⽂件体(file Body)组成。其中，FLV body由⼀对对的(Previous Tag Size字段 + tag)组成。Previous Tag Size字段 排列在Tag之前，占⽤4个字节。Previous Tag Size记录了前⾯⼀个Tag的⼤⼩，⽤于逆向读取处理。FLV header后的第⼀个Pervious Tag Size的值为0。

- FLV header 由如下字段组成，其中前三个字节内容固定是*FLV*，最后4个字节内容固定是9（对*FLV*版本1来说）

    | 字段              | 字段类型 | 字段含义                        |
    | :---------------- | :------: | :------------------------------ |
    | Signature         |   UI8    | 签名，固定为'F' (0x46)          |
    | Signature         |   UI8    | 签名，固定为'L' (0x4c)          |
    | Signature         |   UI8    | 签名，固定为'V' (0x56)          |
    | Version           |   UI8    | 版本，比如 0x01 表示 FLV 版本 1 |
    | TypeFlagsReserved |  UB[5]   | 全为0                           |
    | TypeFlagsAudio    |  UB[1]   | 1表示有audio tag，0表示没有     |
    | TypeFlagsReserved |  UB[1]   | 全为0                           |
    | TypeFlagsVideo    |  UB[1]   | 1表示有video tag，0表示没有     |
    | DataOffset        |   UI32   | FLV header的大小，单位是字节    |

- FLV file body 很有规律，由一系列的*TagSize*和*Tag* 组成，其中*PreviousTagSize0*总是为0；*tag*由*tag header*、*tag body*组成；对*FLV*版本1，*tag header*固定为11个字节，因此，*PreviousTagSize*（除第1个）的值为 11 + 前一个*tag*的*tag body*的大小；

    | 字段               | 字段类型 | 字段含义                        |
    | :----------------- | :------: | :------------------------------ |
    | PreviousTagSize0   |   UI32   | 总是0                           |
    | Tag1               |  FLVTAG  | 第1个tag                        |
    | PreviousTagSize1   |   UI32   | 前一个tag的大小，包括tag header |
    | Tag2               |  FLVTAG  | 第2个tag                        |
    | ...                |   ...    | ...                             |
    | PreviousTagSizeN-1 |   UI32   | 第N-1个tag的大小                |
    | TagN               |  FLVTAG  | 第N个tag                        |
    | PreviousTagSizeN   |   UI32   | 第N个tag的大小，包含tag header  |

- FLV tag 由*tag header*+*tag body*组成。
    *tag header*如下，总共占据11个字节：

    | 字段              |     字段类型      | 字段含义                                                     |
    | :---------------- | :---------------: | :----------------------------------------------------------- |
    | TagType           |        UI8        | tag类型：<br>8：audio<br>9：video<br>18：script data<br>其他：保留 |
    | DataSize          |       UI24        | tag body的大小                                               |
    | Timestamp         |       UI24        | 相对于第一个tag的时间戳（单位是毫秒）第一个tag的Timestamp为0 |
    | TimestampExtended |        UI8        | 时间戳的扩展字段，当 Timestamp 3个字节不够时，会启用这个字段，代表高8位 |
    | StreamID          |       UI24        | 总是0                                                        |
    | Data              | 取决于根据TagType | TagType=8，则为AUDIODATA<br>TagType=9，则为VIDEODATA<br>TagType=18，则为SCRIPTDATAOBJECT |

- Tag⼀般可以分为3种类型：脚本(帧)数据类型、⾳频数据类型、视频数据。FLV数据以⼤端序进⾏存储，在解析时需要注意。

![flv解析过程](/images/flv解析过程.png)

- ⼀个FLV⽂件，每种类型的tag都属于⼀个流，也就是⼀个flv⽂件最多只有⼀个⾳频流，⼀个视频流，不存在多个独⽴的⾳视频流在⼀个⽂件的情况。

- flv⽂件中Timestamp和TimestampExtended拼出来的是dts。也就是解码时间。Timestamp和TimestampExtended拼出来dts单位为ms。(如果不存在B帧，当然dts等于pts)。

- Script data脚本数据就是描述视频或⾳频的信息的数据，如宽度、⾼度、时间等等，⼀个⽂件中通常只有⼀个元数据，⾳频tag和视频tag就是⾳视频信息了，采样、声道、频率，编码等信息。 

- **Script Tag Data结构(脚本类型、帧类型) **

    ![amf](/images/amf.png)
    
    * 该类型Tag⼜被称为MetaData Tag,存放⼀些关于FLV视频和⾳频的元信息，⽐如：duration、width、height等。通常该类型Tag会作为FLV⽂件的第⼀个tag，并且只有⼀个，跟在File Header后。
    
    * AMF包中第一个字节为类型标识：
    
        * Number 0×00;
    
        * Boolean 0×01;
    
        * String 0×02;
    
        * Object 0×03;
    
        * MovieClip 0×04;
    
        * Null 0×05;
    
        * Undefined 0×06;
    
        * Reference 0×07;
    
        * ECMAArray 0×08;
    
        * ObjectEnd 0×09;
    
        * StrictArray 0x0a;
    
        * Date 0x0b;
    
        * LongString 0x0c;
    
        * Unsupported 0x0d;
    
        * Recordset 0x0e;
    
        * XMLObject 0x0f;
    
        * TypedObject(Class) 0×10;
    
* **Audio Tag Data结构(⾳频类型)  **

    - *Audio tags*定义如下所示：

        | 字段        |        字段类型         | 字段含义                                                     |
    | :---------- | :---------------------: | :----------------------------------------------------------- |
        | SoundFormat |          UB[4]          | 音频格式，重点关注 10 = AAC<br>0 = Linear PCM, platform endian<br>1 = ADPCM<br>2 = MP3<br>3 = Linear PCM, little endian<br>4 = Nellymoser 16-kHz mono<br>5 = Nellymoser 8-kHz mono<br>6 = Nellymoser<br>7 = G.711 A-law logarithmic PCM<br>8 = G.711 mu-law logarithmic PCM<br>9 = reserved<br>10 = AAC<br>11 = Speex<br>14 = MP3 8-Khz<br>15 = Device-specific sound |
    | SoundRate   |          UB[2]          | 采样率，对AAC来说，永远等于3<br>0 = 5.5-kHz<br>1 = 11-kHz<br>2 = 22-kHz<br>3 = 44-kHz |
        | SoundSize   |          UB[1]          | 采样精度，对于压缩过的音频，永远是16位<br>0 = snd8Bit<br>1 = snd16Bit |
        | SoundType   |          UB[1]          | 声道类型，对Nellymoser来说，永远是单声道；对AAC来说，永远是双声道；<br>0 = sndMono 单声道<br>1 = sndStereo 双声道 |
        | SoundData   | UI8[size of sound data] | 如果是AAC，则为 AACAUDIODATA；其他请参考规范；               |
    
        *AACAUDIODATA*
        当*SoundFormat*为10时，表示音频采AAC进行编码，此时，*SoundData*的定义如下：
    
        | 字段          | 字段类型 | 字段含义                                                     |
        | :------------ | :------: | :----------------------------------------------------------- |
        | AACPacketType |   UI8    | 0: AAC sequence header<br>1: AAC raw                         |
        | Data          |  UI8[n]  | 如果AACPacketType为0，则为AudioSpecificConfig；如果AACPacketType为1，则为AAC帧数据 |
    
        *AudioSpecificConfig*
    
        | 字段                   | 字段类型 | 字段含义                                           |
        | :--------------------- | :------: | :------------------------------------------------- |
        | AudioObjectType        |  UB[5]   | 编码器类型，比如2表示AAC-LC                        |                                                |
        | SamplingFrequencyIndex |  UB[4]   | 采样率索引值，比如4表示44100                       |
        | ChannelConfiguration   |  UB[4]   | 声道配置，比如2代表双声道，front-left, front-right |
        | AOT Specific Config    |  UB[n]   |                                                    |
    
    * ⾳频Tag Data区域开始的第⼀个字节包含了⾳频数据的参数信息，第⼆个字节开始为⾳频流数据。（这两个字节属于tag的data部分，不是header部分）。
    
    * 第⼆个字节开始为⾳频数据（需要判断该数据是真正的⾳频数据，还是⾳频config信息）。
    
        ![aac audio data](/images/aac audio data.png)
    
* **Video Tag Data结构(视频类型) ** 

    - *Video tags*定义如下：

        | 字段      |   字段类型    | 字段含义                                                     |
        | :-------- | :-----------: | :----------------------------------------------------------- |
        | FrameType |     UB[4]     | 重点关注1、2：<br>1: keyframe (for AVC, a seekable frame) —— 即H.264的IDR帧；<br>2: inter frame (for AVC, a non- seekable frame) —— H.264的普通I帧；<br>3: disposable inter frame (H.263 only)<br>4: generated keyframe (reserved for server use only)<br>5: video info/command frame |
        | CodecID   |     UB[4]     | 编解码器，主要关注 7（AVC）<br>1: JPEG (currently unused)<br>2: Sorenson H.263<br>3: Screen video<br>4: On2 VP6<br>5: On2 VP6 with alpha channel<br>6: Screen video version 2<br>7: AVC |
        | VideoData | 取决于CodecID | 实际的媒体类型，主要关注 7:AVCVIDEOPACKE<br>2: H263VIDEOPACKET<br>3: SCREENVIDEOPACKET<br>4: VP6FLVVIDEOPACKET<br>5: VP6FLVALPHAVIDEOPACKET<br>6: SCREENV2VIDEOPACKET<br>7: AVCVIDEOPACKE |

        *AVCVIDEOPACKE*当*CodecID*为7时，*VideoData*为 *AVCVIDEOPACKE*，也即*H.264*媒体数据。
        *AVCVIDEOPACKE*的定义如下：

        | 字段            | 字段类型 | 字段含义                                                     |
        | :-------------- | :------: | :----------------------------------------------------------- |
        | AVCPacketType   |   UI8    | 0: AVC sequence header<br>1: AVC NALU<br>2: AVC end of sequence |
        | CompositionTime |   SI24   | 如果AVCPacketType=1，则为时间cts偏移量；否则，为0。当B帧的存在时，视频解码呈现过程中，dts、pts可能不同，cts的计算公式为 pts - dts/90，单位为毫秒；如果B帧不存在，则cts固定为0。 |
        | Data            |  UI8[n]  | 1、如果如果AVCPacketType=0，则为AVCDecoderConfigurationRecord，H.264 视频解码所需要的参数集（SPS、PPS）<br>2、如果AVCPacketType=1，则为NALU（一个或多个）<br>3、如果AVCPacketType=2，则为空 |

    - 视频Tag Data开始的第⼀个字节包含视频数据的参数信息，第⼆个字节开始为视频流数据。
    - CompositionTime 表示PTS相对于DTS的偏移值， 在每个视频tag的第14~16字节。显示时间(pts) = 解码时间（tag的第5~8字节） + CompositionTime，CompositionTime的单位也是ms。

## FFmpeg

### FFmpeg命令大全

![FFmpeg命令大全](/images/FFmpeg命令大全.png)

### FFmpeg常用命令

#### FFmpeg基本信息查询命令

![FFmpeg基本信息查询命令](/images/FFmpeg基本信息查询命令.png)

#### 查询设备列表命令

```shell
ffmpeg -f dshow -list_devices true -i dummy
```

#### 录屏命令

``` shell
ffmpeg -f gdigrab -i desktop -r 30 out.mp4
```

#### 分解与复用命令

##### 多媒体格式转换

```shell
ffmpeg -i gx.mkv -vcodec copy -acodec copy out.mp4
```

##### 提取码流

```shell
ffmpeg -i gx.mkv -vcodec copy -an out.264
ffmpeg -i gx.mkv -acodec copy -vn out.aac
```

#### 处理原始数据命令

##### 提取YUV数据

```shell
ffmpeg -i gx.mkv -an -c:v rawvideo -pix_fmt yuv420p out.yuv
```

##### 提取PCM数据

```shell
ffmpeg -i gx.mkv -vn -ar 44100 -ac2 -f s16le out.pcm
```

#### 滤镜命令

* [官方说明](http://ffmpeg.org/ffmpeg-filters.html)

```shell
ffmpeg -i gx.mkv -vf crop=in_w-200:in_h-200 -c:v libx264 -c:a copy out.mp4
```

#### 裁剪与合并命令

##### 裁剪

```shell
ffmpeg -i gx.mkv -ss 00:00:00 -t 10 out.ts
```

##### 合并

```shell
ffmpeg -f concat -i inputs.txt out.flv
```

* inputs.txt文件内容：

```shell
file '1.ts'
file '2.ts'
```

#### 图片/视频转换命令

##### 视频转图片

```shell
ffmpeg -i gx.mkv -r 1 -f image2 image-%3d.jpeg
```

##### 图片转视频

```shell
ffmpeg -i image-%3d.jpeg out.mp4
```

#### 直播相关命令

##### 直播推流

```shell
ffmpeg -re -i gx.mkv -c copy -f flv rtmp://server/live/stream
```

##### 直播拉流

```shell
ffmpeg -i rtmp://58.200.131.2:1935/livetv/hunantv -c copy dump.flv
```

### Windows上编译FFmpeg

* 安装tmd64-gcc

-- [下载地址](https://jmeubank.github.io/tdm-gcc/download/)

*  安装msys

-- [下载地址](https://sourceforge.net/projects/msys/)

* 下载FFmpeg源码

```shell
git clone https://git.ffmpeg.org/ffmpeg.git
```

* 通过vs命令行工具打开msys的启动脚本（msys.bat）。

* 执行编译命令

```shell
./configure --prefix=./msvc2019_32_shared --target-os=win32 --arch=x86 --toolchain=msvc --disable-static --enable-shared --disable-x86asm
make -j 8
make install
```

### FFmpeg API

#### 日志

```shell
#include <libavutil/log.h>
av_log_set_level(AV_LOG_DEBUG);
av_log(nullptr, AV_LOG_INFO, "%s", "some logs");
```

#### 文件操作

```shell
#include <libavformat/avformat.h>
avpriv_io_delete("./test.txt");
avpriv_io_move("src", "dst");
```

#### 目录操作

```shell
#include <libavformat/avformat.h>
avio_open_dir(&ctx, "./", nullptr);
avio_read_dir(ctx, &entry);
avio_free_directory_entry(&entry);
avio_close_dir(&ctx);
```

#### 输出音视频信息

```shell
#include <libavformat/avformat.h>
avformat_open_input(&fmtctx, "gx.mkv", nullptr, nullptr);
av_dump_format(fmtctx, 0, "gx.mkv", 0);
avformat_close_input(&fmtctx);
```

#### 抽取音视频数据

```shell
av_find_best_stream(fmtctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
av_find_best_stream(fmtctx, AVMEDIA_TYPE_VIDIO, -1, -1, NULL, 0);
av_init_packet(&pkt);
av_read_frame(fmtctx, &pkt);
av_packet_unref(&pkt);
av_grow_packet(out, size);
```

#### 封装

```shell
avformat_alloc_output_context2(&fmtctx, nullptr, nullptr, "out.flv");
avformat_free_context(&fmtctx);
avformat_new_stream(fmtctx, nullptr);
avcodec_parameters_copy(outcodecpar, incodecpar);
avformat_write_header(fmtctx, nullptr);
av_write_frame(fmtctx, pkt);
av_interleaved_write_frame(fmtctx, pkt);
av_write_trailer(fmtctx);
```

#### 裁剪

```shell
av_seek_frame(fmtctx, -1, time, AVSEEK_FLAG_ANY);
```

#### 解码

```shell
#include <libavcodec/avcodec.h>
av_frame_alloc();
av_frame_free();
avcodec_alloc_context3();
avcodec_free_context();
avcodec_find_decoder();
avcodec_open2();
avcodec_decode_video2();
avcodec_decode_audio2();
```

#### 编码

```shell
#include <libavcodec/avcodec.h>
avcodec_find_encoder_by_name();
avcodec_alloc_context3();
avcodec_free_context();
av_opt_set();
avcodec_open2();
avcodec_encode_video2();
avcodec_encode_audio2();
```

## SDL

### 编译

```shell
configure --prefix=/build
make -j 8
make install
```

### SDL API

#### 基础

```shell
#include <SDL.h>
SDL_Init();
SDL_Quit();
```

#### 渲染

```shell
SDL_CreateWindow();
SDL_CreateRender();
SDL_CreateTexture();
SDL_SetRenderTarget();
SDL_UpdateTexture();
SDL_UpdateYUVTexture();
SDL_RenderDrawColor();
SDL_RenderDrawRect();
SDL_RenderFillRect();
SDL_RenderClear();
SDL_RenderCopy();
SDL_RenderPresent();
SDL_Delay();
SDL_DesttoyTexture();
SDL_DestroyRenderer();
SDL_DestroyWindow();
```

#### 事件

```shell
SDL_PollEvent();
SDL_WaitEvent();
SDL_PushEvent();
```

#### 线程

```shell
SDL_CreateThread();
```

#### 播放音频

```shell
SDL_OpenAudio();
SDL_CloseAudio();
SDL_PauseAudio();
SDL_MixAudio();
```

## WebRTC

### 架构

![WebRTC架构](/images/WebRTC架构.png)

整体架构分为应用层和核心层。应用层提供实现相关业务逻辑Api，核心层提供应用层需要的核心API。

其中核心层分为四层：

第一层为C++ API，其中最主要的是**PeerConnection**，这个接口需要重点学习和掌握。

第二层为Session层，为上下文管理层，应用里的音频和视频及非音视频的数据处理逻辑都可以在这层进行。

第三层为引擎和传输层，包括音频引擎和视频引擎，以及音视频的传输，这也是整个架构中最重要的一层。

第四层与硬件相关，包括音视频的采集和网络的IO。

注：WebRTC的核心层无视频的渲染，所有的渲染都需要应用层自行实现。

这里着重提一下第三层：

- Voice Engine（音频引擎模块）：包含编码能力、音频缓冲Buff（防网络抖动）、回音消除（实时连麦关键点）。
- Video Engine（视频引擎模块）：包含编码能力（如VP8）、视频缓冲、图像增强。
- Transport（传输模块）：传输协议在底层使用UDP，上层使用的RTP（为安全起见增加SRTP），还有P2P机制，包括STUN、TURN 和 ICE。

### 目录结构

![WebRTC目录结构1](/images/WebRTC目录结构1.png)

![WebRTC目录结构2](/images/module子目录结构2.png)

![module子目录结构1](/images/module子目录结构1.png)

![module子目录结构2](/images/module子目录结构2.png)
