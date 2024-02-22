import express from 'express';
import mongoose from 'mongoose';
import cors from 'cors';

import weatherRoutes from './routes/weather.js';

const app = express();
app.use(express.json());
app.use(cors());

const MONGO_URI = 'mongodb://root:example@localhost:27017';
const PORT = 3333;

mongoose.connect(MONGO_URI).then(() => console.log('Conected to MongoDB'))
.catch(err => console.error('Error', err));

app.use('/weather', weatherRoutes);

app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});
