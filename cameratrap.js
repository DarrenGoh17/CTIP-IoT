import express from 'express';
import mysql from 'mysql';
import bodyParser from 'body-parser';
import cors from 'cors';
import nodemailer from 'nodemailer';


const app = express();
app.use(cors());
app.use(bodyParser.json());

// MySQL Database connection
const db = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: '',
    database: 'semenggoh' // Database should exist or be created by this app
});

// Connect to MySQL Database and set up table
db.connect((err) => {
    if (err) {
        console.error("Database connection failed:", err);
        return;
    }
    console.log('MySQL connected...');

    const createTableSql = `
        CREATE TABLE IF NOT EXISTS sensor_readings (
            id INT AUTO_INCREMENT PRIMARY KEY,
            timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            motion_status VARCHAR(50),
            sound_value INT,
            button_status VARCHAR(50)
        )
    `;
    db.query(createTableSql, (err) => {
        if (err) throw err;
        console.log('Sensor readings table created or already exists');
    });
});



// Email configuration using Nodemailer
const transporter = nodemailer.createTransport({
    service: 'gmail', // Replace with your email service, e.g., 'yahoo', 'outlook'
    auth: {
        user: 'jordandab3366@gmail.com', // Your email address
        pass: 'sslo bzni lmxd mbis'      // Your email app-specific password
    }
});

// Function to send email notifications
function sendEmailNotification(motionStatus) {
    const mailOptions = {
        from: 'jordandab3366@gmail.com',
        to: 'jordanhoo9289@gmail.com',  // Recipient's email address
        subject: 'Motion Detection Alert',
        text: `Alert: ${motionStatus} detected at your location.`
    };

    transporter.sendMail(mailOptions, (error, info) => {
        if (error) {
            console.log('Error sending email:', error);
        } else {
            console.log('Email sent:', info.response);
        }
    });
}

// Handle MQTT messages


// Endpoint to receive HTTP POST data
app.post('/addSensorData', (req, res) => {
    const { motion_status, sound_value, button_status } = req.body;

    if (motion_status === 'Motion Detected') {
        sendEmailNotification(motion_status);
    }

    const sql = `
        INSERT INTO sensor_readings (motion_status, sound_value, button_status) 
        VALUES (?, ?, ?)
    `;
    const values = [motion_status, sound_value, button_status];

    db.query(sql, values, (err) => {
        if (err) {
            console.error('Error inserting sensor data:', err);
            res.status(500).send('Database error');
        } else {
            res.send('Sensor data added successfully');
        }
    });
});

// Start the server
app.listen(3002, () => {
    console.log('Server running on port 3002');
});
