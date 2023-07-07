import React from 'react'
import { useState,useRef,useEffect } from 'react'
import '../scss/chat.scss'
export default function Chat({messages,postMessage }) {
    const [text, setMessage] = useState('')
    const messagesRef = useRef(null)
    const areaHelper = (e) => {
        // console.log(messages);
        setMessage(e.target.value)
    }
    const sendHelper = () => {
        postMessage(text, new Date().toString().slice(0, 21))
    }
    useEffect(()=>{
        messagesRef.current.scrollTo(0,99999)
    },[messages])
    return (
        <>
        <h1> Chat </h1>
        <div className='chat'>
            {/* <div className='chat__users'>
                <b>{`Users (${users.length}):`}</b>
                <ul>
                    {users.length > 0 && users.map((user, index) => <li key={index}>{user}</li>)}
                </ul>
            </div> */}
            <div className='chat__messages'>
                <div ref={messagesRef} className='messages'>
                    {messages && messages.map(message => <div className='message'><p>{message.message}</p> <div><span>{message.characterName +'    '+ message.time}</span></div></div>)}
                    
                </div>
                <div className='chat__field'>
                    <div className="form-group">

                        <textarea onChange={areaHelper} value={text} className="form-control" id="exampleFormControlTextarea1" rows="3"></textarea>
                    </div>
                    <button onClick={sendHelper} type="button" className="btn btn-primary">Send</button>
                </div>
            </div>
        </div>
        </>
    )
}