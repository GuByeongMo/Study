import React, { useState } from 'react';
import './App.css';
import cafeImage from './cafe.jpg';

function App() 
{
  const [korea, setKorea] = useState(true);

  const text = 
  {
    nav: {
      about: korea ? '회사 소개' : 'About Us',
      notice: korea ? '공지사항' : 'Notice',
      faq: korea ? '자주 묻는 질문' : 'FAQ',
    },
    title: korea ? '당신을 위한 독서 커뮤니티' : 'Reading Community for You',
    subtitle: korea ? '뭉글에서 쉽고 간편하게' : 'Easy and Simple with MoongGul',
    buttons: {
      download: korea ? '앱 다운로드' : 'Download App',
      web: korea ? '웹으로 시작하기' : 'Start with Web'
    }
  };

  const changeLang = (isKorea) => {
    setKorea(isKorea);
  };

  return (
    <div className="moonggul-main">
      <header className="moonggul-header">
        <nav>
          <ul>
            <li><button className="nav-btn">{text.nav.about}</button></li>
            <li><button className="nav-btn">{text.nav.notice}</button></li>
            <li><button className="nav-btn">{text.nav.faq}</button></li>
            <li className="lang">
              <button 
                className={`lang-btn ${korea ? 'active' : ''}`} 
                onClick={() => changeLang(true)}
              >
                KOR
              </button>
              {' | '}
              <button 
                className={`lang-btn ${!korea ? 'active' : ''}`} 
                onClick={() => changeLang(false)}
              >
                ENG
              </button>
            </li>
          </ul>
        </nav>
      </header>

      <main className="moonggul-content">
        <h1>
          <span>{text.title}</span>
          <br/>
          <span className="highlight">{text.subtitle}</span>
        </h1>
        <div className="btn-group">
          <button className="main-btn black">
            {text.buttons.download}
          </button>
          <button className="main-btn orange">
            {text.buttons.web}
          </button>
        </div>
        <div className="img-section">
          <img src={cafeImage} alt="카페 이미지" className="cafe-img" />
        </div>
      </main>
    </div>
  );
}

export default App;
