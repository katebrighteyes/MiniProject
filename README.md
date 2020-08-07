# MiniProject

<Step 1>
1. kitti data deploy

압축을 해제해주세요

unzip -qq data_object_image_2.zip

unzip -qq data_object_label_2.zip 

* 원한다면 coco 데이터셋도 다운받아주세요.

2. git clone convert2yolo

https://github.com/ssaru/convert2Yolo

사이트 참조해서 진행해주시면 됩니다.

git clone 한 뒤에 conver 2 yolo 진행해주세요.

git clone https://github.com/ssaru/convert2Yolo.git


3. install requirement of convert2yolo

cd convert2Yolo && pip3 install -r requirements.txt

<Step 2>

4. make kittinames.txt 

아래와 같이 내용을 넣고 파일을 만들어주세요.

Car

Van 

Truck 

Pedestrian 

Person_sitting 

Cyclist 

Tram 

Misc 

DontCare


5. convert kitti to yolo

(예시)

python3 example.py \

--datasets KITTI \

--img_path $KITTI_TRAINING_IMAGE_2_PATH \

--label $KITTI_TRAINING_LABEL_2_PATH \

--convert_output_path $KITTI_TRAINING_IMAGE_2_PATH \

--img_type ".png" \

--manifest_path ~/ \

--cls_list_file kittinames.txt

<Step 3>

6. git clone labelImg

라벨링 툴을 다운 받은 후 라벨링 해주세요.

git clone https://github.com/tzutalin/labelImg.git


7. install labelImg

sudo apt-get install pyqt5-dev-tools

cd labelImg && sudo pip3 install -r requirements/requirements-linux-python3.txt && make qt5py3


8. labeling kitti test dataset

python3 labelImg.py $KITTI_TESTING_IMAGE_2_PATH $PATH_TO_kittinames.txt

