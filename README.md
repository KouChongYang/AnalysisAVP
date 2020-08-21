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

## AAC（高级音频编码）

### AAC格式

* ADIF，音频数据交换格式（Audio Data Interchange Format）。这种格式的特征是可以确定的找到这个音频数据的开始，不需进行在音频数据流中间开始的解码，即它的解码必须在明确定义的开始处进行。故这种格式常用在磁盘文件中。

![ADTS的固定头信息](/images/ADTS的固定头信息.png)

![ADTS的可变头信息](/images/ADTS的可变头信息.png)

* ADTS，音频数据传输流（Audio Data Transport Stream）。这种格式的特征是它是一个有同步字的比特流，解码可以在这个流中任何位置开始。它的特征类似于mp3数据流格式。

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