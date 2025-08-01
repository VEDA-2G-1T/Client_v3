var event_monitoring_type =  '<div class="item_div_cont" data-content="p070303">'+
''+
'    <h3><a class="heading-offset-anchor" id="p070303"></a>이벤트 종류로 검색하기</h3>'+
''+
'    <p>이벤트 탭에 표시되는 실시간 이벤트를 이벤트 종류별로 검색할 수 있습니다.</p>'+
'        <ul class="none-list">'+
'            <li>이벤트 탭의 검색 창에 이벤트 종류를 입력하여 실시간 이벤트를 검색할 수 있습니다.</li>'+
'        </ul>'+
''+
'    <span class="img_preveal_p"><img src="./image/sub/108.png" /></span>'+
''+
'    <ol>'+
'        <li>이벤트 탭에서 <i class="img_preveal_p_i"><img src="./image/sub/i84.svg" /></i>　　를 선택합니다.</li>'+
'        <li>이벤트 종류를 선택합니다.</li>'+
''+
'        <ul class="none-list">'+
'            <li>모든 이벤트: 모든 이벤트를 선택하여 검색합니다.</li>'+
'            <li>움직임 감지: 움직임이 감지되어 발생한 이벤트입니다.</li>'+
'            <li>영상 분석: 지능형 영상 분석이 감지되어 발생한 이벤트입니다.</li>'+
'        </ul>'+
''+
'        <dd class="none-list-ol">'+
'            <ol>'+
'                <li>영상 분석(지나감): 설정한 영역 안에 움직이는 물체가 지나가면 이벤트가 발생합니다.</li>'+
'                <li>영상 분석(침입): 설정한 영역 안에 움직이는 물체를 감지하면 이벤트가 발생합니다.</li>'+
'                <li>영상 분석(들어감): 움직이는 물체가 지정한 영역의 바깥쪽에서 안쪽으로 들어올 때 이벤트가 발생합니다.</li>'+
'                <li>영상 분석(나감): 지정한 영역의 안쪽에서 바깥쪽으로 나갈 때 이벤트가 발생합니다.</li>'+
'                <li>영상 분석(나타남): 지정한 영역 안에 존재하지 않던 물체가 영역 라인을 통과하지 않고 영역 안에 나타나 일정 시간을 머무르면 이벤트가 발생합니다.</li>'+
'                <li>영상 분석(사라짐): 지정한 영역 안에 존재하던 물체가 사라지면 이벤트가 발생합니다.</li>'+
'                <li>탬퍼링 감지: 저장장치에 연결된 카메라 영상이 가려지거나 위치가 변경되면 이벤트가 발생합니다.</li>'+
'                <li>영상 분석(배회): 설정한 가상 영역 안에 배회하는 움직임을 감지하면 이벤트가 발생합니다.</li>'+
'            </ol>'+
'        </dd>'+
''+
'        <ul class="none-list">'+
'            <li>얼굴 감지: 영상에서 얼굴이 감지되어 발생한 이벤트입니다.</li>'+
'            <li>디포커스 감지: 영상의 초점이 흐려지는 것이 감지되어 발생한 이벤트입니다.</li>'+
'            <li>안개 감지: 영상에 안개가 끼거나 흐린 영상이 감지되어 발생한 이벤트입니다.</li>'+
'            <li>오디오 감지: 저장장치에 연결된 카메라에 설정한 레벨 이상의 오디오가 감지되어 발생한 이벤트입니다.</li>'+
'            <li>오디오 분석: 오디오 분석이 감지되어 발생한 이벤트입니다. 오디오 종류 중 <b>비명/총소리/폭발/유리 깨짐</b> 항목을 선택할 수 있습니다.</li>'+
'            <li>자동 추적: 영상에서 움직이는 물체가 추적되어 발생한 이벤트입니다. 자동 추적 시작/종료 시점을 선택할 수 있습니다.</li>'+
'            <li>영상 손실: 저장장치에서 카메라 영상을 수신하지 못하여 발생한 이벤트입니다. 영상 손실/복구 시점을 선택할 수 있습니다.</li>'+
'            <li>알람 입력(카메라): 카메라에서 알람이 입력되어 발생한 이벤트입니다.</li>'+
'            <li>DynamicEvent: 영상에서 특정 움직임이나 상황이 감지되어 발생한 이벤트입니다. <br>SafetyNet Viewer에 등록된 장치에 따라 Dynamic Event 표시 항목이 달라질 수 있습니다.</li>'+
'        </ul>'+
''+
'        <dd class="none-list-ol">'+
'            <ol>'+
'                <li>Queue: 대기열이 감지되어 발생한 이벤트입니다.</li>'+
'                <li>Queue.Level.Medium: 저장장치에 연결된 카메라에 체류 인원수가 중간 수준인 대기열이 감지되어 발생한 이벤트입니다.</li>'+
'                <li>Queue.Level.High: 저장장치에 연결된 카메라에 체류 인원수가 많은 대기열이 감지되어 발생한 이벤트입니다.</li>'+
'                <li>ObjectDetection.Person: 저장장치에 연결된 카메라에서 사람이 감지되어 발생한 이벤트입니다.</li>'+
'                <li>ObjectDetection.Vehicle: 저장장치에 연결된 카메라에서 차량이 감지되어 발생한 이벤트입니다.</li>'+
'                <li>ObjectDetection.Face: 저장장치에 연결된 카메라에서 얼굴이 감지되어 발생한 이벤트입니다.</li>'+
'                <li>ObjectDetection.LicensePlate: 저장장치에 연결된 카메라에서 차량 번호판이 감지되어 발생한 이벤트입니다.</li>'+
'                <li>ShockDetection: 저장장치에 연결된 카메라에 충격이나 진동, 물리적 위치 변화가 감지되어 발생한 이벤트입니다.</li>'+
'                <li>DigitalAutoTracking: 움직이는 특정 물체가 자동 추적되어 발생한 이벤트입니다.</li>'+
'            </ol>'+
'        </dd>'+
''+
''+
'        <li>검색된 이벤트를 더블 클릭하면 영상이 재생됩니다.</li>'+
''+
'        <ul class="none-list">'+
'            <li>현재 레이아웃에 포함된 채널의 이벤트는 해당 타일에서 재생됩니다.<br>레이아웃에 포함되지 않은 채널의 이벤트는 빈 타일에서 재생됩니다.</li>'+
'        </ul>'+
'    </ol>'+
''+
'</div>';