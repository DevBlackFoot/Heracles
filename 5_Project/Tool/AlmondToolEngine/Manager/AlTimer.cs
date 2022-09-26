using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Diagnostics; // stopwatch 사용

namespace AlmondToolEngine.Manager
{
    public class AlTimer
    {
        private Stopwatch _stopwatch;

        private float _deltaTime;   // 초
        private long _baseTime;     // ms 나머진 다 ms
        private long _pausedTime;
        private long _stopTime;
        private long _prevTime;
        private long _currTime;
        private bool _isStoped;

        public float Delta { get { return _deltaTime; } }

        public AlTimer()
        {
            _deltaTime = 0;
            _baseTime = 0;
            _pausedTime = 0;
            _stopTime = 0;
            _prevTime = 0;
            _currTime = 0;
            _isStoped = false;
            _stopwatch = new Stopwatch();

            Reset();
        }

        public void Reset()
        {
            _stopwatch.Reset();
            _stopwatch.Start();
            _baseTime = _stopwatch.ElapsedMilliseconds;
            _prevTime = _stopwatch.ElapsedMilliseconds;
            _stopTime = 0;
            _pausedTime = 0;
            _isStoped = false;
        }

        public void Start()
        {
            // stop과 start 호출 사이 간격 시간 흐름을 누적
            if (!_isStoped)
                return;
            _pausedTime += _stopwatch.ElapsedMilliseconds - _stopTime;
            _prevTime = _stopwatch.ElapsedMilliseconds;
            _stopTime = 0;
            _isStoped = false;
        }

        public void Stop()
        {
            if (_isStoped)
                return;
            _stopTime = _stopwatch.ElapsedMilliseconds;
            _isStoped = true;
        }

        public void Tick()
        {
            if (_isStoped)
            {
                _deltaTime = 0.0f;
                return;
            }
            _currTime = _stopwatch.ElapsedMilliseconds;
            _deltaTime = (_currTime - _prevTime) * 0.0001f;

            // 다음 프레임 준비
            _prevTime = _currTime;
            if (_deltaTime < 0.0f)
                _deltaTime = 0.0f;

        }

        public float TotalTime()
        {
            if (_isStoped)            
                return ((_stopTime - _pausedTime) - _baseTime) * 0.0001f;            
            else            
                return ((_currTime - _pausedTime) - _baseTime) * 0.0001f;            
        }

    }
}
