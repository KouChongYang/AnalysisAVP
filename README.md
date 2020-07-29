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

#### 抽取音频数据

```shell
av_find_best_stream(fmtctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
av_init_packet(&pkt);
av_read_frame(fmtctx, &pkt);
av_packet_unref(&pkt);
```

