var playback_masking_export =  '<div class="item_div_cont" data-content="p080702">'+
''+
'    <h3><a class="heading-offset-anchor" id="p080702"></a>영상 마스킹 설정하기</h3>'+
'    <p>SafetyNet Viewer는 세 가지의 녹화 영상 마스킹 모드를 제공합니다. 한 영상에서 다양한 마스킹 모드를 사용할 수 있습니다.  </p>'+
''+
''+
'    <h4>고정 영역 마스킹하기</h4>'+
'    <p>고정 마스킹은 영상에서 고정된 일부 영역에 대해 마스킹을 적용하는 기능입니다. </p>'+
''+
'    <ol>'+
'        <li>영상 마스킹 창의 왼쪽 상단에서 고정 마스킹 아이콘 <i class="img_preveal_p_i"><img src="./image/sub/masking_fixed_mask_normal.svg" /></i>　　을 선택합니다. </li>'+
'        <li>영상 화면에서 마우스를 드래그하여 마스킹할 영역을 생성합니다.</li>'+
'        <span class="img_preveal_p"><img src="./image/sub/masking_setting_fixed_1.png" /></span>'+  
'        <li>영상의 시작 시간부터 종료 시간까지 설정한 영역에 마스킹이 적용됩니다. 즉 기본적으로 고정 마스킹의 시작 시간과 종료 시간은 영상의 시작 시간 및 종료 시간과 동일하게 설정됩니다. 마스킹 목록에서 마스킹 시간을 수정하면 고정 마스킹 적용 시간을 변경할 수 있습니다.  </li>'+ 
'        <li>마스킹 영역을 마우스로 드래그하여 마스킹 영역을 이동하거나, 마스킹 영역에서 꼭지점을 클릭한 후 드래그하여 마스킹 영역의 크기를 변경할 수 있습니다.  </li>'+ 

'        <li>마스킹 영역을 삭제하려면, 마스킹 영역을 선택한 후 하단에 나타나는 삭제 아이콘 <i class="img_preveal_p_i"><img src="./image/sub/masking_delete_normal.svg" /></i>　　을 클릭합니다.</li>'+
'    </ol>'+

''+
'    <h4>수동으로 객체를 추적하며 마스킹하기</h4>'+
'    <p>수동 마스킹은 마스킹할 객체의 움직임을 수동으로 따라가면서 마스킹 영역을 설정하는 기능입니다. </p>'+

'    <ol>'+
'        <li>영상 마스킹 창의 왼쪽 상단에서 수동 마스킹 아이콘 <i class="img_preveal_p_i"><img src="./image/sub/masking_manual_mask_normal.svg" /></i>　　을 선택합니다.</li>'+
'        <li>영상 화면에서 원하는 객체 위에 마우스를 드래그하여 마스킹 영역을 생성합니다. </li>'+
   
'        <li>마스킹 영역 하단에서 추적 시작 아이콘 <i class="img_preveal_p_i"><img src="./image/sub/masking_tracking_normal.svg" /></i>　　을 클릭한 후 영상이 재생되는 동안 객체의 움직임을 따라가면서 객체를 마스킹합니다. '+
  
'           <ol>'+
'               <li>마스킹 영역을 마우스로 드래그하면 마스킹 영역을 이동할 수 있습니다. </li>'+
'               <li>마스킹 영역의 크기를 변경하려면 꼭지점을 클릭한 후 원하는 방향으로 드래그합니다.  </li>'+
'           </ol>'+
'        <span class="img_preveal_p"><img src="./image/sub/masking_setting_manual_ani.gif" /></span>'+  
'        <li>추적 중지 아이콘<i class="img_preveal_p_i"><img src="./image/sub/masking_tracking_selected.svg" /></i>　　을 클릭하여 영상을 일시 정지한 후 마스킹 영역의 위치를 변경하거나 크기를 변경할 수 있습니다. </li>'+
'        <li>3번과 4번의 과정을 반복하여 객체의 움직임을 수동으로 따라가면서 마스킹 영역을 설정합니다. 이 때 마스킹 영역 하단에서 영상 재생 속도를 x1/2로 설정하면 객체를 추적하기 용이합니다.  </li>'+
'        <li>수동 마스킹을 종료하려면 왼쪽 상단의 수동 마스킹 아이콘<i class="img_preveal_p_i"><img src="./image/sub/masking_manual_mask_selected.svg" /></i>　　을 클릭합니다.  </li>'+

'    </ol>'+

''+
'    <div class="c_bg">'+
'        <h4>참고</h4>'+
'        객체가 마스킹 영역을 벗어난 경우 '+
'        <ol>'+
'        <li>추적 중지 아이콘<i class="img_preveal_p_i"><img src="./image/sub/masking_tracking_selected.svg" /></i>　　을 클릭하여 영상을 일시 중지합니다. </li>'+
'        <li>객체가 마스킹 영역을 벗어난 구간을 타임라인에서 검색한 후, 해당 구간에서 객체를 마스킹하도록 마스킹 영역의 크키나 위치를 변경합니다.</li>'+
'        <span class="img_preveal_p"><img src="./image/sub/masking_setting_manual_3.png" /></span>'+
'        <li>추적 시작 아이콘<i class="img_preveal_p_i"><img src="./image/sub/masking_tracking_normal.svg" /></i>　　을 클릭한 후 객체의 움직임을 수동으로 추적하며 마스킹을 재수행합니다. 객체가 마스킹 영역을 벗어난 구간을 덮어쓸 수 있습니다.  </li>'+
''+

'       </ol>'+
'    </div>'+
''+
'    <h4>자동으로 객체를 추적하며 마스킹하기</h4>'+
'    <p>자동 마스킹은 마스킹할 객체를 지정하면 SafetyNet Viewer가 자동으로 객체를 추적하며 마스킹 영역을 설정하는 기능입니다. </p>'+
''+
'    <ol>'+
'        <li>영상 마스킹 창의 왼쪽 상단에서 자동 마스킹 아이콘<i class="img_preveal_p_i"><img src="./image/sub/masking_auto_mask_normal.svg" /></i>　　을 선택합니다. </li>'+
'        <li>영상 화면에서 원하는 객체 위에 마우스를 드래그하여 마스킹 영역을 생성합니다</li>'+

''+
'        <li>마스킹 영역 하단에서 추적 시작 아이콘<i class="img_preveal_p_i"><img src="./image/sub/masking_tracking_normal.svg" /></i>　　을 클릭합니다. 영상이 재생되는 동안 객체의 움직임에 따라 마스킹 영역이 자동으로 설정됩니다.</li>'+
'        <span class="img_preveal_p"><img src="./image/sub/masking_setting_auto_ani.gif" /></span>'+
'        <li>자동 추적 진행 중 마스킹 영역을 마우스로 드래그하면, 마스킹 영역 위치를 쉽게 변경할 수 있습니다. 이 때 영역을 드래그 후 드롭하는 순간 영상이 일시 중지됩니다. 혹은 추적 중지 아이콘<i class="img_preveal_p_i"><img src="./image/sub/masking_tracking_selected.svg" /></i>　　을 클릭하여 영상을 일시 중지한 후 마스킹 영역의 크기나 위치를 변경할 수도 있습니다. '+
'        <li>추적 시작 아이콘<i class="img_preveal_p_i"><img src="./image/sub/masking_tracking_normal.svg" /></i>　　을 클릭하면 자동 추적이 다시 시작됩니다. </li>'+
'        <li>자동 추적을 종료하려면 자동 추적 아이콘<i class="img_preveal_p_i"><img src="./image/sub/masking_auto_mask_selected.svg" /></i>　　을 클릭합니다. </li>'+

'    </ol>'+
''+
'    <div class="c_bg">'+
'        <h4>참고</h4>'+
'        자동 추적 중 객체가 마스킹 영역을 벗어난 경우 '+
'        <ol>'+
'        <li>추적 중지 아이콘 <i class="img_preveal_p_i"><img src="./image/sub/masking_tracking_selected.svg" /></i>　　을 클릭하여 영상을 일시 중지합니다. </li>'+
'        <li>객체가 마스킹 영역을 벗어난 구간을 타임라인에서 검색한 후, 해당 구간에서 객체를 마스킹하도록 영역의 크기나 위치를 수정합니다. </li>'+
'        <li>추적 시작 아이콘 <i class="img_preveal_p_i"><img src="./image/sub/masking_tracking_normal.svg" /></i>　　을 클릭하여 자동 추적 마스킹을 재수행합니다. 객체가 마스킹 영역을 벗어난 구간을 덮어쓸 수 있습니다.   </li>'+
''+

'       </ol>'+
'    </div>'+
''+
'</div>';