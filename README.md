# MiniProject

<Step 1>
mkdir KITTY folder and 
1. kitti data deploy
unzip -qq data_object_image_2.zip
unzip -qq data_object_label_2.zip 

2. git clone convert2yolo
git clone https://github.com/ssaru/convert2Yolo.git

3. install requirement of convert2yolo
cd convert2Yolo && pip3 install -r requirements.txt

4. make kittinames.txt
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
python3 example.py \
--datasets KITTI \
--img_path $KITTI_TRAINING_IMAGE_2_PATH \
--label $KITTI_TRAINING_LABEL_2_PATH \
--convert_output_path $KITTI_TRAINING_IMAGE_2_PATH \
--img_type ".png" \
--manifest_path ~/ \
--cls_list_file kittinames.txt

6. git clone labelImg
git clone https://github.com/tzutalin/labelImg.git

7. install labelImg
sudo apt-get install pyqt5-dev-tools
cd labelImg && sudo pip3 install -r requirements/requirements-linux-python3.txt && make qt5py3

8. labeling kitti test dataset
python3 labelImg.py $KITTI_TESTING_IMAGE_2_PATH $PATH_TO_kittinames.txt

