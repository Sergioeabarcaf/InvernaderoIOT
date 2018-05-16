import { Component } from '@angular/core';
import { DigiRmService } from '../../service/digi-rm.service';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent {

  constructor(private _digiRMService:DigiRmService) {
    this._digiRMService.getData().subscribe(res => console.log(res));
  }

}
