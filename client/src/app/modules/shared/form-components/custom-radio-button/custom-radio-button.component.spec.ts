import { ComponentFixture, TestBed } from '@angular/core/testing';

import { CustomRadioButtonComponent } from './custom-radio-button.component';

describe('CustomRadioButtonComponent', () => {
  let component: CustomRadioButtonComponent;
  let fixture: ComponentFixture<CustomRadioButtonComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [CustomRadioButtonComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(CustomRadioButtonComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
