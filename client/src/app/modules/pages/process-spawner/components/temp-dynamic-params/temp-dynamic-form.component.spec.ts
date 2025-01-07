import { ComponentFixture, TestBed } from '@angular/core/testing';

import { TempDynamicFormComponent } from './temp-dynamic-form.component';

describe('TempDynamicFormComponent', () => {
  let component: TempDynamicFormComponent;
  let fixture: ComponentFixture<TempDynamicFormComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [TempDynamicFormComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(TempDynamicFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
